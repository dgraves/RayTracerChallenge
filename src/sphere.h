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

#include "matrix44.h"
#include "point.h"

namespace rtc
{
    class Sphere
    {
    public:
        // Default construct a unit sphere.
        Sphere() : transform_(rtc::Matrix44::Identity()), inverse_transform_(rtc::Matrix44::Identity()) {}

        Sphere(const Matrix44& transform) { SetTransform(transform); }

        Sphere(Matrix44&& transform) { SetTransform(std::move(transform)); }

        const Matrix44& GetTransform() const { return transform_; }

        const Matrix44& GetInverseTransform() const { return inverse_transform_; }

        void SetTransform(const Matrix44& transform)
        {
            transform_         = transform;
            inverse_transform_ = rtc::Matrix44::Inverse(transform_);
        }

    private:
        Matrix44 transform_;
        Matrix44 inverse_transform_;
    };
}
