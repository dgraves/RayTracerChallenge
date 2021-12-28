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
#include "double_util.h"
#include "intersect.h"
#include "phong.h"
#include "point.h"
#include "sphere.h"
#include "ray.h"
#include "vector.h"
#include "world.h"

#include <cassert>

namespace rtc
{
    class Computations
    {
    public:
        Computations(double t, const Sphere* object, const Point& point, const Point& over_point, const Vector& eye, const Vector& normal, bool inside) :
            t_(t),
            object_(object),
            point_(point),
            over_point_(over_point),
            eye_(eye),
            normal_(normal),
            inside_(inside)
        {
            assert(object_ != nullptr && "rtc::Computations was initialized with an invalid object");
        }

        Computations(double t, const Sphere* object, Point&& point, Point&& over_point, Vector&& eye, Vector&& normal, bool inside) :
            t_(t),
            object_(object),
            point_(std::move(point)),
            over_point_(std::move(over_point)),
            eye_(std::move(eye)),
            normal_(std::move(normal)),
            inside_(inside)
        {
            assert(object_ != nullptr && "rtc::Computations was initialized with an invalid object");
        }

        double GetT() const { return t_; }

        const Sphere* GetObject() const { return object_; }

        const Point& GetPoint() const { return point_; }

        const Vector& GetEye() const { return eye_; }

        const Vector& GetNormal() const { return normal_; }

        bool IsInside() const { return inside_; }

        const Point& GetOverPoint() const { return over_point_; }

        Color ShadeHit(const World& world) const
        {
            assert(object_ != nullptr && "rtc::Computations was initialized with an invalid object");

            auto color = Color{};

            if (object_ != nullptr)
            {
                const auto& lights = world.GetLights();

                for (const auto& light : lights)
                {
                    color.Add(Phong::Lighting(object_->GetMaterial(), light, over_point_, eye_, normal_, IsShadowed(world, light, over_point_)));
                }
            }

            return color;
        }

        // Instantiate a data strucutre for storing some precomputed values.
        static Computations Prepare(const Intersect::Intersection& intersection, const Ray& ray)
        {
            assert(intersection.object != nullptr && "rtc::Computations::Prepare was called with an invalid rtc::Intersect::Intersection object");

            // TODO: Handle NULL object.
            const auto object = intersection.object;
            auto position     = ray.GetPosition(intersection.t);
            auto eye          = Vector::Negate(ray.GetDirection());
            auto normal       = object->NormalAt(position);

            if (Vector::Dot(normal, eye) < 0)
            {
                normal.Negate();
                auto over_point = rtc::Point{ rtc::Point::Add(position, rtc::Vector::Multiply(normal, rtc::kEpsilon)) };
                return Computations{ intersection.t, object, std::move(position), std::move(over_point), std::move(eye), std::move(normal), true };
            }
            else
            {
                auto over_point = rtc::Point{ rtc::Point::Add(position, rtc::Vector::Multiply(normal, rtc::kEpsilon)) };
                return Computations{ intersection.t, object, std::move(position), std::move(over_point), std::move(eye), std::move(normal), false };
            }
        }

        static Color ColorAt(const World& world, const Ray& ray)
        {
            auto       intersect    = Intersect{ world, ray };
            const auto intersection = intersect.Hit();

            if (intersection != nullptr)
            {
                const auto comps = Computations::Prepare(*intersection, ray);
                return comps.ShadeHit(world);
            }

            return Color{};
        };

        static bool IsShadowed(const World& world, const PointLight& light, const Point& point)
        {
            auto       v        = rtc::Vector{ rtc::Vector::Subtract(light.GetPosition(), point) };
            const auto distance = v.Magnitude();
            v.Normalize(); // Direction

            const auto r             = rtc::Ray{ point, v };
            auto       intersections = rtc::Intersect(world, r);

            const auto h = intersections.Hit();
            if ((h != nullptr) && (h->t < distance))
            {
                return true;
            }

            return false;
        }

    private:
        const double  t_;          ///< Value representing intersection 'time'.
        const Sphere* object_;     ///< Pointer to intersected object.
        const Point   point_;      ///< Position of intersection between ray and object.
        const Point   over_point_; ///< Same as point_ with the z component set to a value slightly less than zero.
        const Vector  eye_;        ///< Eye vector computed from ray.
        const Vector  normal_;     ///< Normal vector at ray intersection point with object.
        const bool    inside_;     ///< Indicates that the intersection is inside the object.
    };
}
