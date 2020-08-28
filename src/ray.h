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

#include "point.h"
#include "vector.h"

namespace rtc
{
    class Ray
    {
    public:
        Ray(const Point& origin, const Vector& direction) :
            origin_(origin),
            direction_(direction)
        {
        }

        Ray(Point&& origin, Vector&& direction) :
            origin_(std::move(origin)),
            direction_(std::move(direction))
        {
        }

        const Point& GetOrigin() const { return origin_; }

        const Vector& GetDirection() const { return direction_; }

        Point GetPosition(double t) const { return rtc::Tuple::Add(origin_, rtc::Tuple::Multiply(direction_, t)); }

    private:
        Point  origin_;
        Vector direction_;
    };
}
