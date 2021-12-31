/*
** Copyright(c) 2020-2021 Dustin Graves
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

#include "matrix.h"
#include "point.h"
#include "ray.h"
#include "tuple.h"
#include "vector.h"

#include <array>
#include <cmath>
#include <stdexcept>

namespace rtc
{
    class Matrix44 : public Matrix<4, 4>
    {
    public:
        Matrix44() = default;

        Matrix44(const Matrix& matrix) :
            Matrix(matrix)
        {
        }

        Matrix44(Matrix&& matrix) :
            Matrix(std::move(matrix))
        {
        }

        Matrix44(const double data[4][4]) :
            Matrix(data)
        {
        }

        Matrix44(std::array<std::array<double, 4>, 4>&& data) :
            Matrix(std::move(data))
        {
        }

        static Matrix44 Identity()
        {
            return Matrix44{ {{
                    {{ 1.0, 0.0, 0.0, 0.0 }},
                    {{ 0.0, 1.0, 0.0, 0.0 }},
                    {{ 0.0, 0.0, 1.0, 0.0 }},
                    {{ 0.0, 0.0, 0.0, 1.0 }}
                    }} };
        }

        static Matrix44 Translation(double x, double y, double z)
        {
            auto transform = Identity();
            transform.Set(0u, 3u, x);
            transform.Set(1u, 3u, y);
            transform.Set(2u, 3u, z);
            return transform;
        }

        static Matrix44 Scaling(double x, double y, double z)
        {
            auto transform = Identity();
            transform.Set(0u, 0u, x);
            transform.Set(1u, 1u, y);
            transform.Set(2u, 2u, z);
            return transform;
        }

        // Rotation will appear to be clockwise around the corresponding axis when
        // viewed along that axis, toward the negative end. (Left-hand rule).
        static Matrix44 RotationX(double rad)
        {
            auto       transform = Identity();
            const auto cos_r     = cos(rad);
            const auto sin_r     = sin(rad);
            transform.Set(1u, 1u, cos_r);
            transform.Set(1u, 2u, -sin_r);
            transform.Set(2u, 1u, sin_r);
            transform.Set(2u, 2u, cos_r);
            return transform;
        }

        static Matrix44 RotationY(double rad)
        {
            auto       transform = Identity();
            const auto cos_r     = cos(rad);
            const auto sin_r     = sin(rad);
            transform.Set(0u, 0u, cos_r);
            transform.Set(0u, 2u, sin_r);
            transform.Set(2u, 0u, -sin_r);
            transform.Set(2u, 2u, cos_r);
            return transform;
        }

        static Matrix44 RotationZ(double rad)
        {
            auto       transform = Identity();
            const auto cos_r     = cos(rad);
            const auto sin_r     = sin(rad);
            transform.Set(0u, 0u, cos_r);
            transform.Set(0u, 1u, -sin_r);
            transform.Set(1u, 0u, sin_r);
            transform.Set(1u, 1u, cos_r);
            return transform;
        }

        static Matrix44 Shearing(double xy, double xz, double yx, double yz, double zx, double zy)
        {
            auto transform = Identity();
            transform.Set(0u, 1u, xy);
            transform.Set(0u, 2u, xz);
            transform.Set(1u, 0u, yx);
            transform.Set(1u, 2u, yz);
            transform.Set(2u, 0u, zx);
            transform.Set(2u, 1u, zy);
            return transform;
        }

        static Matrix44 ViewTransform(const Point& from, const Point& to, const Vector& up)
        {
            const auto forward     = Vector::Normalize(rtc::Vector::Subtract(to, from));
            const auto up_norm     = Vector::Normalize(up);
            const auto left        = Vector::Cross(forward, up_norm);
            const auto true_up     = Vector::Cross(left, forward);
            const auto orientation = Matrix44{ {{
                {{ left.GetX(), left.GetY(), left.GetZ(), 0.0 }},
                {{ true_up.GetX(), true_up.GetY(), true_up.GetZ(), 0.0 }},
                {{ -forward.GetX(), -forward.GetY(), -forward.GetZ(), 0.0 }},
                {{ 0.0, 0.0, 0.0, 1.0 }}
                }} };

            return rtc::Matrix44::Multiply(orientation, rtc::Matrix44::Translation(-from.GetX(), -from.GetY(), -from.GetZ()));
        }

        static Matrix44 Multiply(const Matrix44& lhs, const Matrix44& rhs)
        {
            auto result = Matrix44{};

            for (uint32_t row = 0u; row < 4u; ++row)
            {
                for (uint32_t column = 0u; column < 4u; ++column)
                {
                    auto value =
                        (lhs.Get(row, 0u) * rhs.Get(0u, column)) +
                        (lhs.Get(row, 1u) * rhs.Get(1u, column)) +
                        (lhs.Get(row, 2u) * rhs.Get(2u, column)) +
                        (lhs.Get(row, 3u) * rhs.Get(3u, column));
                    result.Set(row, column, value);
                }
            }

            return result;
        }

        template <typename... Rest>
        static Matrix44 Multiply(const Matrix44& first, const Matrix44& second, Rest... rest)
        {
            return Multiply(Multiply(first, second), rest...);
        }

        static Tuple Multiply(const Matrix44& lhs, const Tuple& rhs)
        {
            auto values = std::array<double, 4>{};

            for (uint32_t row = 0u; row < 4u; ++row)
            {
                values[row] =
                    (lhs.Get(row, 0u) * rhs.GetX()) +
                    (lhs.Get(row, 1u) * rhs.GetY()) +
                    (lhs.Get(row, 2u) * rhs.GetZ()) +
                    (lhs.Get(row, 3u) * rhs.GetW());
            }

            return Tuple(values[0], values[1], values[2], values[3]);
        }

        static Ray Transform(const Ray& ray, const Matrix44& m)
        {
            return Ray(Multiply(m, ray.GetOrigin()), Multiply(m, ray.GetDirection()));
        }

        static bool IsInvertible(const Matrix44& matrix)
        {
            return !rtc::Equal(matrix.Determinant(), 0.0);
        }

        static Matrix44 Inverse(const Matrix44& matrix)
        {
            const auto determinant = matrix.Determinant();
            auto       inverse     = Matrix44{};

            if (!rtc::Equal(determinant, 0.0))
            {
                // Create a matrix consisting of the cofactors of each of the original elements,
                // transposed and divided by the determinant of the original matrix.
                for (uint32_t row = 0u; row < 4u; ++row)
                {
                    for (uint32_t column = 0u; column < 4u; ++column)
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
