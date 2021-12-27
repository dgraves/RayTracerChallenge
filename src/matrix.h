/*
** Copyright(c) 2020 Dustin Graves
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this softwareand associated documentation files(the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions :
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

#pragma once

#include "double_util.h"

#include <algorithm>
#include <array>
#include <cinttypes>
#include <type_traits>

namespace rtc
{
    template <uint32_t Rows, uint32_t Columns>
    class Matrix
    {
    public:
        Matrix() : data_{}
        {
        }

        Matrix(const double data[Rows][Columns])
        {
            for (uint32_t row = 0u; row < Rows; ++row)
            {
                for (uint32_t column = 0u; column < Columns; ++column)
                {
                    data_[row][column] = data[row][column];
                }
            }
        }

        Matrix(std::array<std::array<double, Columns>, Rows>&& data) :
            data_(std::move(data))
        {
        }

        uint32_t NumRows() const { return Rows; }

        uint32_t NumColumns() const { return Columns; }

        void Set(uint32_t row, uint32_t column, double value) { data_[row][column] = value; }

        double Get(uint32_t row, uint32_t column) const { return data_[row][column]; }

        //
        // Operations on the matrix object.
        //

        bool Equal(const Matrix& rhs) const
        {
            for (uint32_t row = 0u; row < Rows; ++row)
            {
                for (uint32_t column = 0u; column < Columns; ++column)
                {
                    if (!rtc::Equal(data_[row][column], rhs.data_[row][column]))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        void Transpose()
        {
            for (uint32_t row = 0u; row < Rows; ++row)
            {
                for (uint32_t column = 0u; column < Columns; ++column)
                {
                    // Stop at diagonal.
                    if (row == column)
                    {
                        break;
                    }

                    std::swap(data_[row][column], data_[column][row]);
                }
            }
        }

        // Remove a row and column from the matrix.
        Matrix<Rows - 1u, Columns - 1u> Submatrix(uint32_t row, uint32_t column) const
        {
            static_assert((Rows > 1u) && (Columns > 1u), "rtc::Matrix::Submatrix requires that both dimensions be greater than 1.");

            auto submatrix = Matrix<Rows - 1u, Columns - 1u>{};

            for (uint32_t sub_row = 0u, current_row = 0u; sub_row < (Rows - 1u); ++sub_row, ++current_row)
            {
                if (current_row == row)
                {
                    ++current_row;
                }

                for (uint32_t sub_column = 0u, current_column = 0u; sub_column < (Columns - 1u); ++sub_column, ++current_column)
                {
                    if (current_column == column)
                    {
                        ++current_column;
                    }

                    submatrix.Set(sub_row, sub_column, data_[current_row][current_column]);
                }
            }

            return submatrix;
        }

        template <uint32_t N = Rows, uint32_t M = Columns>
        typename std::enable_if<((N == M) && (N == 2u)), double>::type Determinant() const
        {
            return (Get(0u, 0u) * Get(1u, 1u)) - (Get(0u, 1u) * Get(1u, 0u));
        }

        template <uint32_t N = Rows, uint32_t M = Columns>
        typename std::enable_if<!((N == M) && (N == 2u)), double>::type Determinant() const
        {
            static_assert((Rows == Columns) && (Rows >= 2u), "rtc::Matrix::Determinant is only implemented for square matrices with dimensions greater than 1x1.");

            auto determinant = 0.0;

            for (uint32_t column = 0u; column < Columns; ++column)
            {
                determinant += Get(0u, column) * Cofactor(0u, column);
            }

            return determinant;
        }

        // Determinant of the submatrix.
        double Minor(uint32_t row, uint32_t column) const
        {
            static_assert((Rows == Columns) && (Rows >= 3u), "rtc::Matrix::Determinant is only implemented for square matrices with dimensions greater than 2x2.");

            const auto submatrix = Submatrix(row, column);
            return submatrix.Determinant();
        }

        // Minor with a potential sign change.
        double Cofactor(uint32_t row, uint32_t column) const
        {
            static_assert((Rows == Columns) && (Rows >= 3u), "rtc::Matrix::Determinant is only implemented for square matrices with dimensions greater than 2x2.");

            auto d = Minor(row, column);

            // If row + column is an odd number, negate the minor.
            if ((row + column) & 0x01u)
            {
                return -d;
            }

            return d;
        }

        //
        // Static operations that may create new matrix objects.
        //

        static bool Equal(const Matrix& lhs, const Matrix& rhs)
        {
            for (uint32_t row = 0u; row < Rows; ++row)
            {
                for (uint32_t column = 0u; column < Columns; ++column)
                {
                    if (!rtc::Equal(lhs.data_[row][column], rhs.data_[row][column]))
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        static Matrix Identity()
        {
            auto identity = Matrix{};

            for (uint32_t row = 0u; row < Rows; ++row)
            {
                for (uint32_t column = 0u; column < Columns; ++column)
                {
                    identity.data_[row][column] = (row != column) ? 0.0 : 1.0;
                }
            }

            return identity;
        }

        static Matrix Transpose(const Matrix& matrix)
        {
            auto transpose = Matrix{};

            for (uint32_t row = 0u; row < Rows; ++row)
            {
                for (uint32_t column = 0u; column < Columns; ++column)
                {
                    transpose.Set(column, row, matrix.Get(row, column));
                }
            }

            return transpose;
        }

        static Matrix<Rows - 1u, Columns - 1u> Submatrix(const Matrix& matrix, uint32_t row, uint32_t column)
        {
            return matrix.Submatrix(row, column);
        }

        static double Minor(const Matrix& matrix, uint32_t row, uint32_t column)
        {
            return matrix.Minor(row, column);
        }

        static double Cofactor(const Matrix& matrix, uint32_t row, uint32_t column)
        {
            return matrix.Cofactor(row, column);
        }

        static double Determinant(const Matrix& matrix)
        {
            return matrix.Determinant();
        }

    private:
        using Row = std::array<double, Columns>;

    private:
        std::array<Row, Rows> data_;
    };
}
