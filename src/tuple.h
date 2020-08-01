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

#include <algorithm>
#include <cinttypes>
#include <cmath>

namespace rtc
{
    const double kEpsilon = 0.00001;
    static inline bool Equal(double l, double r) { return std::abs(l - r) < kEpsilon; }
    static inline double Square(double x) { return x * x; }

    class Tuple
    {
    public:
        Tuple(double x, double y, double z, double w) : x_(x), y_(y), z_(z), w_(w) {}

        double GetX() const { return x_; }

        double GetY() const { return y_; }

        double GetZ() const { return z_; }

        double GetW() const { return w_; }

        bool IsPoint() const { return rtc::Equal(w_, 1.0); }

        bool IsVector() const { return !IsPoint(); }

        //
        // Operations on the tuple object.
        //

        // Compare this tuple object with the specified tuple object. Equivalent to this == rhs.
        bool Equal(const Tuple& rhs)
        {
            return (rtc::Equal(x_, rhs.x_) &&
                    rtc::Equal(y_, rhs.y_) &&
                    rtc::Equal(z_, rhs.z_) &&
                    rtc::Equal(w_, rhs.w_));
        }

        // Negate this tuple object. Equivalent to a -this operation.
        void Negate()
        {
            x_ = -x_;
            y_ = -y_;
            z_ = -z_;
            w_ = -w_;
        }

        // Add the specified tuple object to this tuple object. Equivalent to this + rhs.
        void Add(const Tuple& rhs)
        {
            x_ += rhs.x_;
            y_ += rhs.y_;
            z_ += rhs.z_;
            w_ += rhs.w_;
        }

        // Subtract the specified tuple object from this tuple object. Equivalent to this - rhs.
        void Subtract(const Tuple& rhs)
        {
            x_ -= rhs.x_;
            y_ -= rhs.y_;
            z_ -= rhs.z_;
            w_ -= rhs.w_;
        }

        // Multiply this tuple object with a scalar. Equivalent to this * scalar.
        void Multiply(double scalar)
        {
            x_ *= scalar;
            y_ *= scalar;
            z_ *= scalar;
            w_ *= scalar;
        }

        // Divide this tuple object with a scalar. Equivalent to this / scalar.
        void Divide(double scalar)
        {
            x_ /= scalar;
            y_ /= scalar;
            z_ /= scalar;
            w_ /= scalar;
        }

        //
        // Operations creating a new tuple object.
        //

        static bool Equal(const Tuple& lhs, const Tuple& rhs)
        {
            return (rtc::Equal(lhs.x_, rhs.x_) &&
                    rtc::Equal(lhs.y_, rhs.y_) &&
                    rtc::Equal(lhs.z_, rhs.z_) &&
                    rtc::Equal(lhs.w_, rhs.w_));
        }

        static Tuple Negate(const Tuple& tuple)
        {
            return Tuple(-tuple.x_, -tuple.y_, -tuple.z_, -tuple.w_);
        }

        static Tuple Add(const Tuple& lhs, const Tuple& rhs)
        {
            return Tuple(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_, lhs.w_ + rhs.w_);
        }

        static Tuple Subtract(const Tuple& lhs, const Tuple& rhs)
        {
            return Tuple(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_, lhs.w_ - rhs.w_);
        }

        static Tuple Multiply(const Tuple& tuple, double scalar)
        {
            return Tuple(tuple.x_ * scalar, tuple.y_ * scalar, tuple.z_ * scalar, tuple.w_ * scalar);
        }

        static Tuple Divide(const Tuple& tuple, double scalar)
        {
            return Tuple(tuple.x_ / scalar, tuple.y_ / scalar, tuple.z_ / scalar, tuple.w_ / scalar);
        }

    private:
        double x_;
        double y_;
        double z_;
        double w_;
    };

    class Point : public Tuple
    {
    public:
        Point(double x, double y, double z) : Tuple(x, y, z, 1.0) {}
    };

    class Vector : public Tuple
    {
    public:
        Vector(double x, double y, double z) : Tuple(x, y, z, 0.0) {}

        // Compute the magnitude of the vector.
        double Magnitude() const
        {
            return sqrt(rtc::Square(GetX()) + rtc::Square(GetY()) + rtc::Square(GetZ()) + rtc::Square(GetW()));
        }

        // Normalize the vector.
        void Normalize()
        {
            double magnitude = Magnitude();
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

        static Vector Normalize(const Vector& vector)
        {
            double magnitude = vector.Magnitude();
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

        static Vector Cross(const Vector& lhs, const Vector& rhs)
        {
            return Vector((lhs.GetY() * rhs.GetZ()) - (lhs.GetZ() * rhs.GetY()),
                          (lhs.GetZ() * rhs.GetX()) - (lhs.GetX() * rhs.GetZ()),
                          (lhs.GetX() * rhs.GetY()) - (lhs.GetY() * rhs.GetX()));
        }
    };
}
