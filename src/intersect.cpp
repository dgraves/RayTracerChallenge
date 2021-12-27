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

#include "intersect.h"

#include "double_util.h"
#include "matrix44.h"
#include "tuple.h"
#include "vector.h"

#include <algorithm>
#include <cmath>

namespace rtc
{
    static bool IntersectionCompare(const Intersect::Intersection& lhs, const Intersect::Intersection& rhs)
    {
        return (lhs.t < rhs.t);
    }

    void Intersect::TestIntersect(const Sphere* sphere, const Ray& ray)
    {
        if (sphere == nullptr)
        {
            return;
        }

        // Transform ray to the sphere's object space.
        const auto transformed_ray = rtc::Matrix44::Transform(ray, sphere->GetInverseTransform());

        // Compute the vector from the sphere's center to the transformed ray's origin.
        // The sphere's center is at (0, 0, 0), so a vector constructed from the ray's origin
        // is the same as the vector produced by (ray_origin - sphere_origin).
        const auto sphere_to_ray = rtc::Vector{ transformed_ray.GetOrigin() };
        const auto ray_direction = transformed_ray.GetDirection();

        const auto a = rtc::Vector::Dot(ray_direction, ray_direction);
        const auto b = 2.0 * rtc::Vector::Dot(ray_direction, sphere_to_ray);
        const auto c = rtc::Vector::Dot(sphere_to_ray, sphere_to_ray) - 1.0;

        const auto discriminant = rtc::Square(b) - 4.0 * a * c;

        // When discriminant is less than 0, the ray did not intersect the sphere.
        if (discriminant < 0.0)
        {
            return;
        }

        // Compute intersetcion 'times'.  For the tangent case, return the same value twice.
        const auto two_a  = 1.0 / (2.0 * a);
        const auto sqrt_d = sqrt(discriminant);
        const auto t1     = (-b - sqrt_d) * two_a;
        const auto t2     = (-b + sqrt_d) * two_a;

        // Insert in sorted order.
        if (t1 < t2)
        {
            values_.emplace_back(Intersection{ t1, sphere });
            values_.emplace_back(Intersection{ t2, sphere });
        }
        else
        {
            values_.emplace_back(Intersection{ t2, sphere });
            values_.emplace_back(Intersection{ t1, sphere });
        }
    }

    void Intersect::TestIntersect(const World& world, const Ray& ray)
    {
        const auto count = world.GetObjectCount();

        for (size_t i = 0u; i < count; ++i)
        {
            const auto& object = world.GetObject(i);
            TestIntersect(&object, ray);
        }

        if (!values_.empty())
        {
            Sort(values_);
        }
    }

    void Intersect::Sort(Intersections& intersections)
    {
        // Maintain original ordering of intersections with the same t value.
        std::stable_sort(intersections.begin(), intersections.end(), IntersectionCompare);
    }

    const Intersect::Intersection* Intersect::Hit(const Intersections& intersections)
    {
        const Intersect::Intersection* hit = nullptr;

        if (!intersections.empty())
        {
            // Find the first non-negative value.  Because hits are sorted by t,
            // this is also the first hit.
            for (size_t i = 0u; i < intersections.size(); ++i)
            {
                const auto current = &intersections[i];

                if (current->t >= 0)
                {
                    hit = current;
                    break;
                }
            }
        }

        return hit;
    }
}
