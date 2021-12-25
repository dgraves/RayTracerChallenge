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

#include "canvas.h"
#include "matrix44.h"
#include "ray.h"
#include "world.h"

namespace rtc
{
    class Camera
    {
    public:
        Camera(uint32_t hsize, uint32_t vsize, double field_of_view) :
            hsize_(hsize),
            vsize_(vsize),
            field_of_view_(field_of_view),
            transform_(rtc::Matrix44::Identity()),
            inverse_transform_(rtc::Matrix44::Identity())
        {
            ComputeSizes(hsize, vsize, field_of_view);
        }

        Camera(double hsize, double vsize, double field_of_view, const Matrix44& transform) :
            hsize_(hsize),
            vsize_(vsize),
            field_of_view_(field_of_view),
            transform_(transform),
            inverse_transform_(rtc::Matrix44::Inverse(transform))
        {
            ComputeSizes(hsize, vsize, field_of_view);
        }

        Camera(double hsize, double vsize, double field_of_view, Matrix44&& transform) :
            hsize_(hsize),
            vsize_(vsize),
            field_of_view_(field_of_view),
            transform_(std::move(transform))
        {
            inverse_transform_ = rtc::Matrix44::Inverse(transform_);
            ComputeSizes(hsize, vsize, field_of_view);
        }

        uint32_t GetHSize() const { return hsize_; }

        uint32_t GetVSize() const { return vsize_; }

        double GetFieldOfView() const { return field_of_view_; }

        double GetHalfWidth() const { return half_width_; }

        double GetHalfHeight() const { return half_height_; }

        double GetPixelSize() const { return pixel_size_; }

        const Matrix44& GetTransform() const { return transform_; }

        void SetTransform(const Matrix44& transform)
        {
            transform_ = transform;
            inverse_transform_ = rtc::Matrix44::Inverse(transform_);
        }

        void SetTransform(Matrix44&& transform)
        {
            transform_ = std::move(transform);
            inverse_transform_ = rtc::Matrix44::Inverse(transform_);
        }

        Ray RayForPixel(uint32_t px, uint32_t py) const;

        Canvas Render(const World& world) const;

    private:
        void ComputeSizes(uint32_t hsize, uint32_t vsize, double field_of_view);

    private:
        uint32_t hsize_;             ///< The horizontal size, in pixels, of the canvas.
        uint32_t vsize_;             ///< The vertical size, in pixels, of the canvas.
        double field_of_view_;       ///< The angle that describes how much the camera can see.
        double half_width_;          ///< Half the width of the canvas in world-space units.
        double half_height_;         ///< Half the height of the canvas in world-space units.
        double pixel_size_;          ///< Size, in world-space units, of the pixels on the canvas.
        Matrix44 transform_;         ///< The matrix describing how the world should be oriented relative to the camera.
        Matrix44 inverse_transform_; ///< The inverted transformation matrix.
    };
}
