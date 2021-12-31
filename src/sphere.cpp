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

#include "sphere.h"

#include "double_util.h"

#include <cmath>

namespace rtc
{
    void Sphere::LocalIntersect(const Ray& local_ray, Intersections::Values& values) const
    {
        // Compute the vector from the sphere's center to the transformed ray's origin.
        // The sphere's center is at (0, 0, 0), so a vector constructed from the ray's origin
        // is the same as the vector produced by (ray_origin - sphere_origin).
        const auto sphere_to_ray = Vector{ local_ray.GetOrigin() };
        const auto ray_direction = local_ray.GetDirection();

        const auto a = Vector::Dot(ray_direction, ray_direction);
        const auto b = 2.0 * Vector::Dot(ray_direction, sphere_to_ray);
        const auto c = Vector::Dot(sphere_to_ray, sphere_to_ray) - 1.0;

        const auto discriminant = Square(b) - 4.0 * a * c;

        // When discriminant is less than 0, the ray did not intersect the sphere.
        if (discriminant < 0.0)
        {
            return;
        }

        // Compute intersetcion 'times'.  For the tangent case, return the same value twice.
        const auto two_a = 1.0 / (2.0 * a);
        const auto sqrt_d = sqrt(discriminant);
        const auto t1 = (-b - sqrt_d) * two_a;
        const auto t2 = (-b + sqrt_d) * two_a;

        // Insert in sorted order.
        if (t1 < t2)
        {
            values.emplace_back(t1, shared_from_this());
            values.emplace_back(t2, shared_from_this());
        }
        else
        {
            values.emplace_back(t2, shared_from_this());
            values.emplace_back(t1, shared_from_this());
        }
    }
}
