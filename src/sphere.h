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

#include "intersections.h"
#include "material.h"
#include "matrix44.h"
#include "point.h"
#include "shape.h"
#include "ray.h"
#include "vector.h"

namespace rtc
{
    class Sphere : public Shape
    {
    public:
        // Default construct a unit sphere.
        Sphere() :
            Shape()
        {
        }

        Sphere(const Material& material) :
            Shape(material)
        {
        }

        Sphere(Material&& material) :
            Shape(std::move(material))
        {
        }

        Sphere(const Matrix44& transform) :
            Shape(transform)
        {
        }

        Sphere(Matrix44&& transform) :
            Shape(std::move(transform))
        {
        }

        Sphere(const Material& material, const Matrix44& transform) :
            Shape(material, transform)
        {
        }

        Sphere(Material&& material, Matrix44&& transform) :
            Shape(std::move(material), std::move(transform))
        {
        }

    private:
        virtual void LocalIntersect(const Ray& local_ray, Intersections::Values& values) const override;

        virtual Vector LocalNormalAt(const Point& local_point) const override
        {
            return Vector{ local_point };
        }
    };
}
