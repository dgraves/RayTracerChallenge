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

#include "camera.h"
#include "computations.h"
#include "point.h"
#include "vector.h"

#include <cmath>

namespace rtc
{
    Ray Camera::RayForPixel(uint32_t px, uint32_t py) const
    {
        // Compute the offset from the edge of the canvas to the pixel's center.
        auto xoffset = (static_cast<double>(px) + 0.5) * pixel_size_;
        auto yoffset = (static_cast<double>(py) + 0.5) * pixel_size_;

        // Compute the untransformed coordinates of the pixel in world space.
        // Note that the camera looks toward -z, so +x is to the *left*.
        auto world_x = half_width_ - xoffset;
        auto world_y = half_height_ - yoffset;

        // Using the camera matrix, transform the canvas point and the origin,
        // and then compute the ray's direction vector.
        // Note that the canvas is at z = -1.
        auto pixel     = Matrix44::Multiply(inverse_transform_, Point(world_x, world_y, -1.0));
        auto origin    = Matrix44::Multiply(inverse_transform_, Point(0.0, 0.0, 0.0));
        auto direction = Vector::Normalize(Vector::Subtract(pixel, origin));

        return Ray(origin, direction);
    }

    Canvas Camera::Render(const World& world) const
    {
        auto image = Canvas(hsize_, vsize_);

        for (uint32_t y = 0u; y < vsize_; ++y)
        {
            for (uint32_t x = 0u; x < hsize_; ++x)
            {
                auto ray   = RayForPixel(x, y);
                auto color = Computations::ColorAt(world, ray);
                image.WritePixel(x, y, std::move(color));
            }
        }

        return image;
    }

    void Camera::ComputeSizes(uint32_t hsize, uint32_t vsize, double field_of_view)
    {
        auto half_view = tan(field_of_view / 2.0);
        auto aspect    = static_cast<double>(hsize) / static_cast<double>(vsize);

        if (aspect >= 1.0)
        {
            half_width_  = half_view;
            half_height_ = half_view / aspect;
        }
        else
        {
            half_width_  = half_view * aspect;
            half_height_ = half_view;
        }

        // Assuming square pixels, where horizontal size is equal to vertical size, so
        // vertical size does not need to be computed separately.
        pixel_size_ = (half_width_ * 2.0) / hsize;
    }
}
