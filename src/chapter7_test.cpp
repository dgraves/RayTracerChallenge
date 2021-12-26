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

#include "catch2/catch.hpp"

#include "camera.h"
#include "color.h"
#include "computations.h"
#include "double_util.h"
#include "intersect.h"
#include "material.h"
#include "matrix44.h"
#include "point_light.h"
#include "point.h"
#include "sphere.h"
#include "world.h"

SCENARIO("Creating a world", "[world]")
{
    GIVEN("w <- world()")
    {
        auto w = rtc::World{};

        THEN("w contains no objects and w has no light source")
        {
            REQUIRE(w.GetObjectCount() == 0);
            REQUIRE(w.GetLightCount() == 0);
        }
    }
}

SCENARIO("The default world", "[world]")
{
    GIVEN("light <- point_light(point(-10, 10, -10), color(1, 1, 1))\n"
          "and s1 <- sphere() with:\n"
          "  | material.color    | (0.8, 1.0, 0.6) |\n"
          "  | material.diffuse  | 0.7             |\n"
          "  | material.specular | 0.2             |\n"
          "and s2 <- sphere() with:\n"
          "  | transform | scaling(0.5, 0.5, 0.5) |\n")
    {
        auto light = rtc::PointLight(rtc::Point(-10, 10, -10), rtc::Color(1, 1, 1));
        auto s1    = rtc::Sphere(rtc::Material(
            rtc::Color(0.8, 1.0, 0.6),
            rtc::Material::GetDefaultAmbient(),
            0.7,
            0.2,
            rtc::Material::GetDefaultShininess()));
        auto s2 = rtc::Sphere(rtc::Matrix44::Scaling(0.5, 0.5, 0.5));

        WHEN("w <- default_world()")
        {
            auto w = rtc::World::GetDefault();

            THEN("w.light = light and w contains s1 and w contains s2")
            {
                REQUIRE(rtc::PointLight::Equal(w.GetLight(0), light));
                REQUIRE(rtc::Sphere::Equal(w.GetObject(0), s1));
                REQUIRE(rtc::Sphere::Equal(w.GetObject(1), s2));
            }
        }
    }
}

SCENARIO("Intersect a world with a ray", "[world]")
{
    GIVEN("w <- default_world() and r <- ray(point(0, 0, -5), vector(0, 0, 1))")
    {
        auto w = rtc::World::GetDefault();
        auto r = rtc::Ray(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));

        WHEN("xs <- intersect_world(w, r)")
        {
            auto xs = rtc::Intersect(w, r);

            THEN("xs.count = 4 and xs[0].t = 4 and xs[1].t = 4.5 and xs[2].t = 5.5 and xs[3].t = 6")
            {
                REQUIRE(xs.GetCount() == 4);
                REQUIRE(rtc::Equal(xs.GetValue(0).t, 4));
                REQUIRE(rtc::Equal(xs.GetValue(1).t, 4.5));
                REQUIRE(rtc::Equal(xs.GetValue(2).t, 5.5));
                REQUIRE(rtc::Equal(xs.GetValue(3).t, 6));
            }
        }
    }
}

SCENARIO("Precomputing the state of an intersection", "[world]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and shape <- sphere() and i <- intersection(4, shape)")
    {
        auto r     = rtc::Ray(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
        auto shape = rtc::Sphere{};
        auto i     = rtc::Intersect::Intersection{ 4, &shape };

        WHEN("comps <- prepare_computations(i, r)")
        {
            auto comps = rtc::Computations::Prepare(i, r);

            THEN("comps.t = i.t and comps.object = i.object and comps.point = point(0, 0, -1) and comps.eyev = vector(0, 0, -1) and comps.normalv = vector(0, 0, -1)")
            {
                REQUIRE(rtc::Equal(comps.GetT(), i.t));
                REQUIRE(comps.GetObject() == i.object);
                REQUIRE(rtc::Vector::Equal(comps.GetPoint(), rtc::Point(0, 0, -1)));
                REQUIRE(rtc::Vector::Equal(comps.GetEye(), rtc::Vector(0, 0, -1)));
                REQUIRE(rtc::Vector::Equal(comps.GetNormal(), rtc::Vector(0, 0, -1)));
            }
        }
    }
}

SCENARIO("The hit, when an intersection occurs on the outside", "[world]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and shape <- sphere() and i <- intersection(4, shape)")
    {
        auto r     = rtc::Ray(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
        auto shape = rtc::Sphere{};
        auto i     = rtc::Intersect::Intersection{ 4, &shape };

        WHEN("comps <- prepare_computations(i, r)")
        {
            auto comps = rtc::Computations::Prepare(i, r);

            THEN("comps.inside = false")
            {
                REQUIRE(!comps.IsInside());
            }
        }
    }
}

SCENARIO("The hit, when an intersection occurs on the inside", "[world]")
{
    GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1)) and shape <- sphere() and i <- intersection(1, shape)")
    {
        auto r     = rtc::Ray(rtc::Point(0, 0, 0), rtc::Vector(0, 0, 1));
        auto shape = rtc::Sphere{};
        auto i     = rtc::Intersect::Intersection{ 1, &shape };

        WHEN("comps <- prepare_computations(i, r)")
        {
            auto comps = rtc::Computations::Prepare(i, r);

            THEN("comps.point = point(0, 0, 1) and comps.eyev = vector(0, 0, -1) and comps.inside = true and comps.normalv = vector(0, 0, -1)")
            {
                REQUIRE(rtc::Vector::Equal(comps.GetPoint(), rtc::Point(0, 0, 1)));
                REQUIRE(rtc::Vector::Equal(comps.GetEye(), rtc::Vector(0, 0, -1)));
                REQUIRE(comps.IsInside());
                REQUIRE(rtc::Vector::Equal(comps.GetNormal(), rtc::Vector(0, 0, -1)));
            }
        }
    }
}

SCENARIO("Shading an intersection", "[world]")
{
    GIVEN("w <- default_world() and r <- ray(point(0, 0, -5), vector(0, 0, 1)) and shape <- the first object in w and i <- intersection(4, shape)")
    {
        auto w     = rtc::World::GetDefault();
        auto r     = rtc::Ray(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
        auto shape = w.GetObject(0);
        auto i     = rtc::Intersect::Intersection{ 4, &shape };

        WHEN("comps <- prepare_computations(i, r) and c <- shade_hit(w, comps)")
        {
            auto comps = rtc::Computations::Prepare(i, r);
            auto c     = comps.ShadeHit(w);

            THEN("c = color(0.38066, 0.47583, 0.2855)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color(0.38066, 0.47583, 0.2855)));
            }
        }
    }
}

SCENARIO("Shading an intersection from the inside", "[world]")
{
    GIVEN("w <- default_world() and w.light = point_light(point(0, 0.25, 0), color(1, 1, 1)) and r <- ray(point(0, 0, 0), vector(0, 0, 1)) and shape <- the second object in w and i <- intersection(0.5, shape)")
    {
        auto w = rtc::World::GetDefault();
        w.SetLight(0, rtc::PointLight(rtc::Point(0, 0.25, 0), rtc::Color(1, 1, 1)));

        auto r     = rtc::Ray(rtc::Point(0, 0, 0), rtc::Vector(0, 0, 1));
        auto shape = w.GetObject(1);
        auto i     = rtc::Intersect::Intersection{ 0.5, &shape };

        WHEN("comps <- prepare_computations(i, r) and c <- shade_hit(w, comps)")
        {
            auto comps = rtc::Computations::Prepare(i, r);
            auto c     = comps.ShadeHit(w);

            THEN("c = color(0.90498, 0.90498, 0.90498)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color(0.90498, 0.90498, 0.90498)));
            }
        }
    }
}

SCENARIO("The color when a ray misses", "[world]")
{
    GIVEN("w <- default_world() and r <- ray(point(0, 0, -5), vector(0, 1, 0))")
    {
        auto w = rtc::World::GetDefault();
        auto r = rtc::Ray(rtc::Point(0, 0, -5), rtc::Vector(0, 1, 0));

        WHEN("c <- color_at(w, r)")
        {
            auto c = rtc::Computations::ColorAt(w, r);

            THEN("c = color(0, 0, 0)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color(0, 0, 0)));
            }
        }
    }
}

SCENARIO("The color when a ray hits", "[world]")
{
    GIVEN("w <- default_world() and r <- ray(point(0, 0, -5), vector(0, 0, 1))")
    {
        auto w = rtc::World::GetDefault();
        auto r = rtc::Ray(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));

        WHEN("c <- color_at(w, r)")
        {
            auto c = rtc::Computations::ColorAt(w, r);

            THEN("c = color(0.38066, 0.47583, 0.2855)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color(0.38066, 0.47583, 0.2855)));
            }
        }
    }
}

SCENARIO("The color with an intersection behind the ray", "[world]")
{
    GIVEN("w <- default_world() and outer <- the first object in w and outer.material.ambient <- 1 and inner <- the second object in w <- inner.material.ambient <- 1 and r <- ray(point(0, 0, 0.75), vector(0, 0, -1))")
    {
        auto w     = rtc::World::GetDefault();
        auto outer = w.GetObject(0);
        auto inner = w.GetObject(1);

        auto outer_material = outer.GetMaterial();
        outer_material.SetAmbient(1);
        outer.SetMaterial(outer_material);
        w.SetObject(0, outer);

        auto inner_material = inner.GetMaterial();
        inner_material.SetAmbient(1);
        inner.SetMaterial(inner_material);
        w.SetObject(1, inner);

        auto r = rtc::Ray(rtc::Point(0, 0, 0.75), rtc::Vector(0, 0, -1));

        WHEN("c <- color_at(w, r)")
        {
            auto c = rtc::Computations::ColorAt(w, r);

            THEN("c = inner.material.color")
            {
                REQUIRE(rtc::Color::Equal(c, inner_material.GetColor()));
            }
        }
    }
}

SCENARIO("The transformation matrix for the default orientation", "[world]")
{
    GIVEN("from <- point(0, 0, 0) and to <- point(0, 0, -1) and up <- vector(0, 1, 0)")
    {
        WHEN("t <- view_transform(from, to, up)")
        {
            THEN("t = identity_matrix")
            {
                auto from = rtc::Point(0, 0, 0);
                auto to   = rtc::Point(0, 0, -1);
                auto up   = rtc::Vector(0, 1, 0);
                auto t    = rtc::Matrix44::ViewTransform(from, to, up);

                REQUIRE(rtc::Matrix44::Equal(t, rtc::Matrix44::Identity()));
            }
        }
    }
}

SCENARIO("A view transformation matrix looking in positive z direction", "[world]")
{
    GIVEN("from <- point(0, 0, 0) and to <- point(0, 0, 1) and up <- vector(0, 1, 0)")
    {
        auto from = rtc::Point(0, 0, 0);
        auto to   = rtc::Point(0, 0, 1);
        auto up   = rtc::Vector(0, 1, 0);

        WHEN("t <- view_transform(from, to, up)")
        {
            auto t = rtc::Matrix44::ViewTransform(from, to, up);

            THEN("t = scaling(-1, 1, -1)")
            {
                REQUIRE(rtc::Matrix44::Equal(t, rtc::Matrix44::Scaling(-1, 1, -1)));
            }
        }
    }
}

SCENARIO("The view transformation moves the world", "[world]")
{
    GIVEN("from <- point(0, 0, 8) and to <- point(0, 0, 0) and up <- vector(0, 1, 0)")
    {
        auto from = rtc::Point(0, 0, 8);
        auto to   = rtc::Point(0, 0, 0);
        auto up   = rtc::Vector(0, 1, 0);

        WHEN("t <- view_transform(from, to, up)")
        {
            auto t = rtc::Matrix44::ViewTransform(from, to, up);

            THEN("t = translation(0, 0, -8)")
            {
                REQUIRE(rtc::Matrix44::Equal(t, rtc::Matrix44::Translation(0, 0, -8)));
            }
        }
    }
}

SCENARIO("An arbitrary view transformation", "[world]")
{
    GIVEN("from <- point(1, 3, 2) and to <- point(4, -2, 8) and up <- vector(1, 1, 0)")
    {
        auto from = rtc::Point(1, 3, 2);
        auto to   = rtc::Point(4, -2, 8);
        auto up   = rtc::Vector(1, 1, 0);

        WHEN("t <- view_transform(from, to, up)")
        {
            auto t = rtc::Matrix44::ViewTransform(from, to, up);

            THEN("t is the following 4x4 matrix:\n"
                 "  | -0.50709 | 0.50709 |  0.67612 | -2.36643 |\n"
                 "  |  0.76772 | 0.60609 |  0.12122 | -2.82843 |\n"
                 "  | -0.35857 | 0.59761 | -0.71714 |  0.00000 |\n"
                 "  |  0.00000 | 0.00000 |  0.00000 |  1.00000 |")
            {
                auto expected = rtc::Matrix44({{
                    {{ -0.50709, 0.50709, 0.67612, -2.36643 }},
                    {{ 0.76772, 0.60609, 0.12122, -2.82843 }},
                    {{ -0.35857, 0.59761, -0.71714, 0.00000 }},
                    {{ 0.00000, 0.00000, 0.00000, 1.00000 }}
                    }});

                REQUIRE(rtc::Matrix44::Equal(t, expected));
            }
        }
    }
}

SCENARIO("Constructing a camera", "[world]")
{
    GIVEN("hsize <- 160 and vsize <- 120 and field_of_view <- pi/2")
    {
        auto hsize         = 160u;
        auto vsize         = 120u;
        auto field_of_view = rtc::kPi / 2.0;

        WHEN("c <- camera(hsize, vsize, field_of_view)")
        {
            auto c = rtc::Camera(hsize, vsize, field_of_view);

            THEN("c.hsize = 160 and c.vsize = 120 and c.field_of_view = pi/2 and c.transform = identity_matrix")
            {
                REQUIRE(rtc::Equal(c.GetHSize(), hsize));
                REQUIRE(rtc::Equal(c.GetVSize(), vsize));
                REQUIRE(rtc::Equal(c.GetFieldOfView(), field_of_view));
                REQUIRE(rtc::Matrix44::Equal(c.GetTransform(), rtc::Matrix44::Identity()));
            }
        }
    }
}

SCENARIO("The pixel size for a horizontal canvas", "[world]")
{
    GIVEN("c <- camera(200, 125, pi/2)")
    {
        auto c = rtc::Camera(200u, 125u, rtc::kPi / 2.0);

        THEN("c.pixel_size = 0.01")
        {
            REQUIRE(rtc::Equal(c.GetPixelSize(), 0.01));
        }
    }
}

SCENARIO("The pixel size for a vertical canvas", "[world]")
{
    GIVEN("c <- camera(125, 200, pi/2)")
    {
        auto c = rtc::Camera(125u, 200u, rtc::kPi / 2.0);

        THEN("c.pixel_size = 0.01")
        {
            REQUIRE(rtc::Equal(c.GetPixelSize(), 0.01));
        }
    }
}

SCENARIO("Constructing a ray through the center of the canvas", "[world]")
{
    GIVEN("c <- camera(201, 101, pi/2)")
    {
        auto c = rtc::Camera(201u, 101u, rtc::kPi / 2.0);

        WHEN("r <- ray_for_pixel(c, 100, 50)")
        {
            auto r = c.RayForPixel(100u, 50u);

            THEN("r.origin = point(0, 0, 0) and r.direction = vector(0, 0, -1)")
            {
                REQUIRE(rtc::Point::Equal(r.GetOrigin(), rtc::Point(0, 0, 0)));
                REQUIRE(rtc::Vector::Equal(r.GetDirection(), rtc::Vector(0, 0, -1)));
            }
        }
    }
}

SCENARIO("Constructing a ray through a corner of the canvas", "[world]")
{
    GIVEN("c <- camera(201, 101, pi/2)")
    {
        auto c = rtc::Camera(201u, 101u, rtc::kPi / 2.0);

        WHEN("r <- ray_for_pixel(c, 0, 0)")
        {
            auto r = c.RayForPixel(0u, 0u);

            THEN("r.origin = point(0, 0, 0) and r.direction = vector(0.66519, 0.33259, -0.66851)")
            {
                REQUIRE(rtc::Point::Equal(r.GetOrigin(), rtc::Point(0, 0, 0)));
                REQUIRE(rtc::Vector::Equal(r.GetDirection(), rtc::Vector(0.66519, 0.33259, -0.66851)));
            }
        }
    }
}

SCENARIO("Constructing a ray when the camera is transformed", "[world]")
{
    GIVEN("c <- camera(201, 101, pi/2)")
    {
        auto c = rtc::Camera(201, 101, rtc::kPi / 2.0);

        WHEN("c.transform <- rotation_y(pi/4) * translation(0, -2, 5) and r <- ray_for_pixel(c, 100, 50)")
        {
            c.SetTransform(rtc::Matrix44::Multiply(rtc::Matrix44::RotationY(rtc::kPi / 4.0), rtc::Matrix44::Translation(0, -2, 5)));

            THEN("r.origin = point(0, 2, -5) and r.direction = vector(sqrt(2)/2, 0, -sqrt(2)/2)")
            {
                auto r = c.RayForPixel(100u, 50u);

                REQUIRE(rtc::Point::Equal(r.GetOrigin(), rtc::Point(0, 2, -5)));
                REQUIRE(rtc::Vector::Equal(r.GetDirection(), rtc::Vector(sqrt(2.0) / 2.0, 0, -sqrt(2.0) / 2.0)));
            }
        }
    }
}

SCENARIO("Rendering a world with a camera", "[world]")
{
    GIVEN("w <- default_world() and c <- camera(11, 11, pi/2) and from <- point(0, 0, -5) and to <- point(0, 0, 0) and up <- vector(0, 1, 0) and c.transform <- view_transform(from, to , up)")
    {
        auto w    = rtc::World::GetDefault();
        auto from = rtc::Point(0, 0, -5);
        auto to   = rtc::Point(0, 0, 0);
        auto up   = rtc::Vector(0, 1, 0);
        auto c    = rtc::Camera(11u, 11u, rtc::kPi / 2.0, rtc::Matrix44::ViewTransform(from, to, up));

        WHEN("image <- render(c, w)")
        {
            auto image = c.Render(w);

            THEN("pixel_at(image, 5, 5) = color(0.38066, 0.47583, 0.2855)")
            {
                REQUIRE(rtc::Color::Equal(image.PixelAt(5u, 5u), rtc::Color(0.38066, 0.47583, 0.2855)));
            }
        }
    }
}
