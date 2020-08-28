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

#include "matrix.h"
#include "ray.h"
#include "tuple.h"

#include <cmath>
#include <stdexcept>

namespace rtc
{
    class Matrix44 : public Matrix<4, 4>
    {
    public:
        Matrix44()
        {
        }

        Matrix44(const Matrix& matrix) :
            Matrix(matrix)
        {
        }

        Matrix44(Matrix&& matrix) :
            Matrix(std::move(matrix))
        {
        }

        Matrix44(double data[4][4]) :
            Matrix(data)
        {
        }

        Matrix44(std::array<std::array<double, 4>, 4>&& data) :
            Matrix(std::move(data))
        {
        }

        static Matrix44 Identity()
        {
            return Matrix44({{
                    {{ 1, 0, 0, 0 }},
                    {{ 0, 1, 0, 0 }},
                    {{ 0, 0, 1, 0 }},
                    {{ 0, 0, 0, 1 }}
                    }});
        }

        static Matrix44 Translation(double x, double y, double z)
        {
            Matrix44 transform = Identity();
            transform.Set(0, 3, x);
            transform.Set(1, 3, y);
            transform.Set(2, 3, z);
            return transform;
        }

        static Matrix44 Scaling(double x, double y, double z)
        {
            Matrix44 transform = Identity();
            transform.Set(0, 0, x);
            transform.Set(1, 1, y);
            transform.Set(2, 2, z);
            return transform;
        }

        // Rotation will appear to be clockwise around the corresponding axis when
        // viewed along that axis, toward the negative end. (Left-hand rule).
        static Matrix44 RotationX(double rad)
        {
            Matrix44 transform = Identity();
            double cos_r = cos(rad);
            double sin_r = sin(rad);
            transform.Set(1, 1, cos_r);
            transform.Set(1, 2, -sin_r);
            transform.Set(2, 1, sin_r);
            transform.Set(2, 2, cos_r);
            return transform;
        }

        static Matrix44 RotationY(double rad)
        {
            Matrix44 transform = Identity();
            double cos_r = cos(rad);
            double sin_r = sin(rad);
            transform.Set(0, 0, cos_r);
            transform.Set(0, 2, sin_r);
            transform.Set(2, 0, -sin_r);
            transform.Set(2, 2, cos_r);
            return transform;
        }

        static Matrix44 RotationZ(double rad)
        {
            Matrix44 transform = Identity();
            double cos_r = cos(rad);
            double sin_r = sin(rad);
            transform.Set(0, 0, cos_r);
            transform.Set(0, 1, -sin_r);
            transform.Set(1, 0, sin_r);
            transform.Set(1, 1, cos_r);
            return transform;
        }

        static Matrix44 Shearing(double xy, double xz, double yx, double yz, double zx, double zy)
        {
            Matrix44 transform = Identity();
            transform.Set(0, 1, xy);
            transform.Set(0, 2, xz);
            transform.Set(1, 0, yx);
            transform.Set(1, 2, yz);
            transform.Set(2, 0, zx);
            transform.Set(2, 1, zy);
            return transform;
        }

        static Matrix44 Multiply(const Matrix& lhs, const Matrix& rhs)
        {
            Matrix44 result;

            for (uint32_t row = 0; row < 4; ++row)
            {
                for (uint32_t column = 0; column < 4; ++column)
                {
                    double value =
                        (lhs.Get(row, 0) * rhs.Get(0, column)) +
                        (lhs.Get(row, 1) * rhs.Get(1, column)) +
                        (lhs.Get(row, 2) * rhs.Get(2, column)) +
                        (lhs.Get(row, 3) * rhs.Get(3, column));
                    result.Set(row, column, value);
                }
            }

            return result;
        }

        static Tuple Multiply(const Matrix& lhs, const Tuple& rhs)
        {
            double values[4];

            for (uint32_t row = 0; row < 4; ++row)
            {
                values[row] =
                    (lhs.Get(row, 0) * rhs.GetX()) +
                    (lhs.Get(row, 1) * rhs.GetY()) +
                    (lhs.Get(row, 2) * rhs.GetZ()) +
                    (lhs.Get(row, 3) * rhs.GetW());
            }

            return Tuple(values[0], values[1], values[2], values[3]);
        }

        static Ray Transform(const Ray& ray, const Matrix& m)
        {
            return Ray(Multiply(m, ray.GetOrigin()), Multiply(m, ray.GetDirection()));
        }

        static bool IsInvertible(const Matrix44& matrix)
        {
            return !rtc::Equal(matrix.Determinant(), 0.0);
        }

        static Matrix44 Inverse(const Matrix44& matrix)
        {
            double   determinant = matrix.Determinant();
            Matrix44 inverse;

            if (!rtc::Equal(determinant, 0.0))
            {
                // Create a matrix consisting of the cofactors of each of the original elements,
                // transposed and divided by the determinant of the original matrix.
                for (uint32_t row = 0; row < 4; ++row)
                {
                    for (uint32_t column = 0; column < 4; ++column)
                    {
                        inverse.Set(column, row, matrix.Cofactor(row, column) / determinant);
                    }
                }
            }
            else
            {
                throw std::runtime_error("Attempt to invert a non-invertible matrix");
            }

            return inverse;
        }
    };
}
