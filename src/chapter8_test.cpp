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

#include "catch2/catch.hpp"

#include "color.h"
#include "computations.h"
#include "double_util.h"
#include "intersection.h"
#include "material.h"
#include "matrix44.h"
#include "phong.h"
#include "point.h"
#include "point_light.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"
#include "world.h"

SCENARIO("Lighting with the surface in shadow", "[shadows]")
{
    GIVEN("m <- material and position <- point(0, 0, 0) and eyev <- vector(0, 0, -1) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 0, -10), color(1, 1, 1)) and in_shadow <- true")
    {
        const auto m         = rtc::Material{};
        const auto position  = rtc::Point{ 0.0, 0.0, 0.0 };
        const auto eyev      = rtc::Vector{ 0.0, 0.0, -1.0 };
        const auto normalv   = rtc::Vector{ 0.0, 0.0, -1.0 };
        const auto light     = rtc::PointLight{ rtc::Point{ 0.0, 0.0, -10.0 }, rtc::Color{ 1.0, 1.0, 1.0 } };
        const auto in_shadow = true;

        WHEN("result <- lighting(m, light, position, eyev, normalv)")
        {
            const auto result = rtc::Phong::Lighting(m, light, position, eyev, normalv, in_shadow);

            THEN("result = color(0.1, 0.1, 0.1)")
            {
                REQUIRE(rtc::Color::Equal(result, rtc::Color{ 0.1, 0.1, 0.1 }));
            }
        }
    }
}

SCENARIO("There is no shadow when nothing is collinear with point and light", "[shadows]")
{
    GIVEN("w <- default_world() and p <- point(0, 10, 0)")
    {
        const auto w = rtc::World::GetDefault();
        const auto p = rtc::Point{ 0.0, 10.0, 0.0 };

        THEN("is_shadowed(w, p) is false")
        {
            REQUIRE(!rtc::Computations::IsShadowed(w, w.GetLight(0), p));
        }
    }
}

SCENARIO("The shadow when an object is between the point and the light", "[shadows]")
{
    GIVEN("w <- default_world() and p <- point(10, -10, 10)")
    {
        const auto w = rtc::World::GetDefault();
        const auto p = rtc::Point{ 10.0, -10.0, 10.0 };

        THEN("is_shadowed(w, p) is false")
        {
            REQUIRE(rtc::Computations::IsShadowed(w, w.GetLight(0), p));
        }
    }
}

SCENARIO("There is no shadow when an object is behind the light", "[shadows]")
{
    GIVEN("w <- default_world() and p <- point(20, -20, 20)")
    {
        const auto w = rtc::World::GetDefault();
        const auto p = rtc::Point{ -20.0, 20.0, -20.0 };

        THEN("is_shadowed(w, p) is false")
        {
            REQUIRE(!rtc::Computations::IsShadowed(w, w.GetLight(0), p));
        }
    }
}

SCENARIO("There is no shadow when an object is behind the point", "[shadows]")
{
    GIVEN("w <- default_world() and p <- point(2, -2, 2)")
    {
        const auto w = rtc::World::GetDefault();
        const auto p = rtc::Point{ -2.0, 2.0, -2.0 };

        THEN("is_shadowed(w, p) is false")
        {
            REQUIRE(!rtc::Computations::IsShadowed(w, w.GetLight(0), p));
        }
    }
}

SCENARIO("shade_hit() is given an intersection in shadow", "[shadows]")
{
    GIVEN("w <- default_world() and \n"
          "w.light <- point_light(point(0, 0, -10), color(1, 1, 1)) and \n"
          "s1 <- sphere() and \n"
          "s1 is added to w and \n"
          "s2 <- sphere() with: \n"
          "  | transform | translation(0, 0, 10) | and \n"
          "s2 is added to w and \n"
          "r <- ray(point(0, 0, 5), vector(0, 0, 1)) and \n"
          "i <- intersection(4, s2)")
    {
        const auto w = rtc::World{
            { rtc::PointLight{ rtc::Point{ 0.0, 0.0, -10.0 }, rtc::Color{1.0, 1.0, 1.0} } },
            {
                rtc::Sphere::Create(),
                rtc::Sphere::Create(rtc::Matrix44::Translation(0.0, 0.0, 10.0))
            } };
        const auto& s2 = w.GetObject(1);
        const auto r   = rtc::Ray{ rtc::Point{ 0.0, 0.0, 5.0 }, rtc::Vector{ 0.0, 0.0, 1.0 } };
        const auto i   = rtc::Intersection{ 4.0, s2 };

        WHEN("comps <- prepare_computations(i, r) and c <- shade_hit(w, comps)")
        {
            const auto comps = rtc::Computations::Prepare(i, r);
            const auto c = comps.ShadeHit(w);

            THEN("c = color(0.1, 0.1, 0.1)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color{ 0.1, 0.1, 0.1 }));
            }
        }
    }
}

SCENARIO("The hit should offset the point", "[shadows]")
{
    GIVEN("r <-ray(point(0, 0, -5), vector(0, 0, 1)) and \n"
          "shape <- sphere() with: \n"
          "  | transform | translation(0, 0, 1) | and \n"
          "i <- intersection(5, shape)")
    {
        const auto r     = rtc::Ray{ rtc::Point{ 0.0, 0.0, -5.0 }, rtc::Vector{ 0.0, 0.0, 1.0 } };
        auto       shape = rtc::Sphere::Create(rtc::Matrix44::Translation(0.0, 0.0, 1.0));
        const auto i     = rtc::Intersection{ 5.0, std::move(shape) };

        WHEN("comps <- prepare_computations(i, r)")
        {
            const auto comps = rtc::Computations::Prepare(i, r);

            THEN("comps.over_point.z < -EPSILON/2")
            {
                REQUIRE(comps.GetOverPoint().GetZ() < (-rtc::kEpsilon / 2.0));
            }
        }
    }
}
