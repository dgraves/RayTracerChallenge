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

#include "color.h"
#include "point.h"

namespace rtc
{
    class PointLight
    {
    public:
        PointLight(const Point& position, const Color& intensity) :
            position_(position),
            intensity_(intensity)
        {
        }

        PointLight(Point&& position, Color&& intensity) :
            position_(std::move(position)),
            intensity_(std::move(intensity))
        {
        }

        const Point& GetPosition() const { return position_; }

        const Color& GetIntensity() const { return intensity_; }

        static bool Equal(const PointLight& lhs, const PointLight& rhs)
        {
            return (Point::Equal(lhs.position_, rhs.position_) &&
                    Color::Equal(lhs.intensity_, rhs.intensity_));
        }

    private:
        Point position_;   ///< Position of the light.
        Color intensity_;  ///< Brightness and color of the light.
    };
}
