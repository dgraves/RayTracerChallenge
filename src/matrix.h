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
** The above copyright noticeand this permission notice shall be included in all
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

namespace rtc
{
    template <uint32_t Rows, uint32_t Columns>
    class Matrix
    {
    public:
        Matrix() : data_{}
        {
        }

        Matrix(double data[Rows][Columns])
        {
            for (uint32_t row = 0; row < Rows; ++row)
            {
                for (uint32_t column = 0; column < Columns; ++column)
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
            for (uint32_t row = 0; row < Rows; ++row)
            {
                for (uint32_t column = 0; column < Columns; ++column)
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
            for (uint32_t row = 0; row < Rows; ++row)
            {
                for (uint32_t column = 0; column < Columns; ++column)
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

        Matrix<Rows - 1, Columns - 1> Submatrix(uint32_t row, uint32_t column) const
        {
            Matrix<Rows - 1, Columns - 1> submatrix;

            for (uint32_t sub_row = 0, current_row = 0; sub_row < (Rows - 1); ++sub_row, ++current_row)
            {
                if (current_row == row)
                {
                    ++current_row;
                }

                for (uint32_t sub_column = 0, current_column = 0; sub_column < (Columns - 1); ++sub_column, ++current_column)
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

        //
        // Operations creating a new matrix object.
        //

        static bool Equal(const Matrix& lhs, const Matrix& rhs)
        {
            for (uint32_t row = 0; row < Rows; ++row)
            {
                for (uint32_t column = 0; column < Columns; ++column)
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
            Matrix identity;

            for (uint32_t row = 0; row < Rows; ++row)
            {
                for (uint32_t column = 0; column < Columns; ++column)
                {
                    identity.data_[row][column] = (row != column) ? 0.0 : 1.0;
                }
            }

            return identity;
        }

        static Matrix Transpose(const Matrix& matrix)
        {
            Matrix transpose;

            for (uint32_t row = 0; row < Rows; ++row)
            {
                for (uint32_t column = 0; column < Columns; ++column)
                {
                    transpose.Set(column, row, matrix.Get(row, column));
                }
            }

            return transpose;
        }

        static Matrix<Rows - 1, Columns - 1> Submatrix(const Matrix& matrix, uint32_t row, uint32_t column)
        {
            return matrix.Submatrix(row, column);
        }

    private:
        typedef std::array<double, Columns> Row;

    private:
        std::array<Row, Rows> data_;
    };
}
