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
#include "tuple.h"

#include <cassert>
#include <utility>

namespace rtc
{
    class Color : public Tuple
    {
    public:
        Color() : Tuple(0.0, 0.0, 0.0, 0.0) {}

        Color(const Tuple& tuple) : Tuple(tuple) { assert(rtc::Equal(GetW(), 0.0)); }

        Color(Tuple&& tuple) : Tuple(std::move(tuple)) { assert(rtc::Equal(GetW(), 0.0)); }

        Color(double r, double g, double b) : Tuple(r, g, b, 0.0) {}

        double GetR() const { return GetX(); }

        double GetG() const { return GetY(); }

        double GetB() const { return GetZ(); }

        static Color HadamardProduct(const Color& lhs, const Color& rhs)
        {
            return Color(
                lhs.GetR() * rhs.GetR(),
                lhs.GetG() * rhs.GetG(),
                lhs.GetB() * rhs.GetB());
        }
    };
}
