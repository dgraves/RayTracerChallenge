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

#include "point.h"
#include "tuple.h"

#include <cassert>
#include <cmath>

namespace rtc
{
    class Vector : public Tuple
    {
    public:
        Vector() : Tuple(0.0, 0.0, 0.0, 0.0) {}

        Vector(const Tuple& tuple) : Tuple(tuple) { assert(IsVector()); }

        Vector(Tuple&& tuple) : Tuple(std::move(tuple)) { assert(IsVector()); }

        Vector(const Point& point) : Tuple(point.GetX(), point.GetY(), point.GetZ(), 0.0) {}

        Vector(double x, double y, double z) : Tuple(x, y, z, 0.0) {}

        // Compute the magnitude of the vector.
        double Magnitude() const
        {
            return sqrt(rtc::Square(GetX()) + rtc::Square(GetY()) + rtc::Square(GetZ()) + rtc::Square(GetW()));
        }

        // Normalize the vector.
        void Normalize()
        {
            const auto magnitude = Magnitude();
            if (!rtc::Equal(magnitude, 0.0))
            {
                Divide(magnitude);
            }
        }

        // Compute the dot product between two vectors (cosine of angle between them).  Equivalend to this . vector.
        double Dot(const Vector& vector)
        {
            return ((GetX() * vector.GetX()) + (GetY() * vector.GetY()) + (GetZ() * vector.GetZ()));
        }

        // Reflect the vector around a normal vector.
        void Reflect(const Vector& normal)
        {
            Subtract(Multiply(normal, 2.0 * Dot(normal)));
        }

        static Vector Normalize(const Vector& vector)
        {
            const auto magnitude = vector.Magnitude();
            if (!rtc::Equal(magnitude, 0.0))
            {
                return Vector(vector.GetX() / magnitude, vector.GetY() / magnitude, vector.GetZ() / magnitude);
            }
            else
            {
                return vector;
            }
        }

        static double Dot(const Vector& lhs, const Vector& rhs)
        {
            return ((lhs.GetX() * rhs.GetX()) + (lhs.GetY() * rhs.GetY()) + (lhs.GetZ() * rhs.GetZ()));
        }

        static Vector Reflect(const Vector& in, const Vector& normal)
        {
            return Subtract(in, Multiply(normal, 2.0 * Dot(in, normal)));
        }

        static Vector Cross(const Vector& lhs, const Vector& rhs)
        {
            return Vector((lhs.GetY() * rhs.GetZ()) - (lhs.GetZ() * rhs.GetY()),
                          (lhs.GetZ() * rhs.GetX()) - (lhs.GetX() * rhs.GetZ()),
                          (lhs.GetX() * rhs.GetY()) - (lhs.GetY() * rhs.GetX()));
        }
    };
}
