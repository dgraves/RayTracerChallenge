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

#include <cinttypes>
#include <cstdlib>

namespace rtc
{
    constexpr double kEpsilon = 0.00001;

    constexpr double kPi = 3.141592653589793238462643383279502884;

    inline bool Equal(double l, double r) { return std::abs(l - r) < kEpsilon; }

    constexpr double Square(double d) { return d * d; }

    constexpr uint8_t ToByte(double d)
    {
        auto b = static_cast<int32_t>(255.0 * d);
        return static_cast<uint8_t>((b < 0) ? 0 : (b > 255) ? 255 : b);
    }

    constexpr double DegreesToRadians(double d) { return d * (kPi / 180.0); }

    constexpr double RadiansToDegrees(double r) { return r * (180.0 / kPi); }
}
