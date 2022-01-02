/*
** Copyright(c) 2021 Dustin Graves
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

#include "color.h"
#include "double_util.h"
#include "matrix44.h"
#include "pattern.h"
#include "point.h"

#include <cmath>

namespace rtc
{
    class CheckersPattern : public Pattern
    {
    public:
        template <typename... Args>
        static std::shared_ptr<CheckersPattern> Create(Args... args)
        {
            return std::shared_ptr<CheckersPattern>(new CheckersPattern(args...));
        }

    public:
        const Color& GetA() const { return a_; }

        const Color& GetB() const { return b_; }

        virtual Color PatternAt(const Point& point) const override
        {
            if (static_cast<uint64_t>(floor(point.GetX()) + floor(point.GetY()) + floor(point.GetZ())) % 2 == 0)
            {
                return a_;
            }

            return b_;
        }

    protected:
        CheckersPattern(const Color& a, const Color& b) :
            a_(a),
            b_(b)
        {
        }

        CheckersPattern(Color&& a, Color&& b) :
            a_(std::move(a)),
            b_(std::move(b))
        {
        }

        CheckersPattern(const Color& a, const Color& b, const Matrix44& transform) :
            Pattern(transform),
            a_(a),
            b_(b)
        {
        }

        CheckersPattern(Color&& a, Color&& b, Matrix44&& transform) :
            Pattern(std::move(transform)),
            a_(std::move(a)),
            b_(std::move(b))
        {
        }

    private:
        Color a_;
        Color b_;
    };
}
