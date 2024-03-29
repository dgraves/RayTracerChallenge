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
#include "matrix44.h"
#include "point.h"

#include <memory>

namespace rtc
{
    class Pattern : public std::enable_shared_from_this<Pattern>
    {
    public:
        virtual ~Pattern() = default;

        const Matrix44& GetTransform() const { return transform_; }

        void SetTransform(const Matrix44& transform)
        {
            transform_ = transform;
            inverse_transform_ = Matrix44::Inverse(transform_);
        }

        void SetTransform(Matrix44&& transform)
        {
            transform_ = std::move(transform);
            inverse_transform_ = Matrix44::Inverse(transform_);
        }

        virtual Color PatternAt(const Point& point) const = 0;

        Color PatternAtObject(const Matrix44& object_inverse_transform, const Point& world_point) const
        {
            const auto object_point  = Matrix44::Multiply(object_inverse_transform, world_point);
            const auto pattern_point = Matrix44::Multiply(inverse_transform_, object_point);
            return PatternAt(pattern_point);
        }

    protected:
        Pattern() :
            transform_(Matrix44::Identity()),
            inverse_transform_(Matrix44::Identity())
        {
        }

        Pattern(const Matrix44& transform) :
            transform_(transform),
            inverse_transform_(Matrix44::Inverse(transform))
        {
        }

        Pattern(Matrix44&& transform) :
            transform_(std::move(transform))
        {
            inverse_transform_ = Matrix44::Inverse(transform_);
        }

    private:
        Matrix44 transform_;
        Matrix44 inverse_transform_;
    };
}
