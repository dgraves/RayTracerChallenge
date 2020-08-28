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

#include "intersect.h"

#include "double_util.h"
#include "matrix44.h"
#include "tuple.h"
#include "vector.h"

#include <cmath>

namespace rtc
{
    bool Intersect::TestIntersect(Sphere* sphere, const Ray& ray)
    {
        if (sphere == nullptr)
        {
            return false;
        }

        // Transform ray to the sphere's object space.
        rtc::Ray transformed_ray = rtc::Matrix44::Transform(ray, sphere->GetInverseTransform());

        // Compute the vector from the sphere's center to the transformed ray's origin.
        // The sphere's center is at (0, 0, 0), so a vector constructed from the ray's origin
        // is the same as the vector produced by (ray_origin - sphere_origin).
        rtc::Vector sphere_to_ray(transformed_ray.GetOrigin());
        rtc::Vector ray_direction = transformed_ray.GetDirection();

        double a = rtc::Vector::Dot(ray_direction, ray_direction);
        double b = 2.0 * rtc::Vector::Dot(ray_direction, sphere_to_ray);
        double c = rtc::Vector::Dot(sphere_to_ray, sphere_to_ray) - 1.0;

        double discriminant = rtc::Square(b) - 4.0 * a * c;

        // When discriminant is less than 0, the ray did not intersect the sphere.
        if (discriminant < 0.0)
        {
            return false;
        }

        // Compute intersetcion 'times'.  For the tangent case, return the same value twice.
        double two_a  = 1.0 / (2.0 * a);
        double sqrt_d = sqrt(discriminant);
        double t1 = (-b - sqrt_d) * two_a;
        double t2 = (-b + sqrt_d) * two_a;

        values_.emplace_back(Intersection{ t1, sphere });
        values_.emplace_back(Intersection{ t2, sphere });

        return true;
    }

    const Intersect::Intersection* Intersect::Hit(const Intersections& intersections)
    {
        const Intersect::Intersection* hit = nullptr;

        if (!intersections.empty())
        {
            size_t i = 0;

            // Find the first non-negative value.
            for (; i < intersections.size(); ++i)
            {
                auto current = &intersections[i];

                if (current->t >= 0)
                {
                    hit = current;
                    break;
                }
            }

            // If there were no non-negative values in the intersection list, hit will be nullptr.
            if (hit != nullptr)
            {
                ++i;
                for (; i < intersections.size(); ++i)
                {
                    auto current = &intersections[i];

                    if ((current->t >= 0) && (current->t < hit->t))
                    {
                        hit = current;
                    }
                }
            }
        }

        return hit;
    }
}
