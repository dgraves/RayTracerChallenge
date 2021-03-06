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

#include "catch2/catch.hpp"

#include "color.h"
#include "double_util.h"
#include "material.h"
#include "matrix44.h"
#include "phong.h"
#include "point.h"
#include "point_light.h"
#include "sphere.h"
#include "vector.h"

#include <cmath>

SCENARIO("The normal on a sphere at a point on the x axis", "[light and shading]")
{
    GIVEN("s <- sphere()")
    {
        WHEN("n <- normal_at(s, point(1, 0, 0)")
        {
            THEN("n = vector(1, 0, 0)")
            {
                rtc::Sphere s;
                rtc::Vector n = s.NormalAt(rtc::Point(1, 0, 0));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector(1, 0, 0)));
            }
        }
    }
}

SCENARIO("The normal on a sphere at a point on the y axis", "[light and shading]")
{
    GIVEN("s <- sphere()")
    {
        WHEN("n <- normal_at(s, point(0, 1, 0)")
        {
            THEN("n = vector(0, 1, 0)")
            {
                rtc::Sphere s;
                rtc::Vector n = s.NormalAt(rtc::Point(0, 1, 0));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector(0, 1, 0)));
            }
        }
    }
}

SCENARIO("The normal on a sphere at a point on the z axis", "[light and shading]")
{
    GIVEN("s <- sphere()")
    {
        WHEN("n <- normal_at(s, point(0, 0, 1)")
        {
            THEN("n = vector(0, 0, 1)")
            {
                rtc::Sphere s;
                rtc::Vector n = s.NormalAt(rtc::Point(0, 0, 1));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector(0, 0, 1)));
            }
        }
    }
}

SCENARIO("The normal on a sphere at a non-axial point", "[light and shading]")
{
    GIVEN("s <- sphere()")
    {
        WHEN("n <- normal_at(s, point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0)")
        {
            THEN("n = vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0)")
            {
                rtc::Sphere s;
                rtc::Vector n = s.NormalAt(rtc::Point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0)));
            }
        }
    }
}

SCENARIO("The normal is a normalized vector", "[light and shading]")
{
    GIVEN("s <- sphere()")
    {
        WHEN("n <- normal_at(s, point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0)")
        {
            THEN("n = vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0)")
            {
                rtc::Sphere s;
                rtc::Vector n = s.NormalAt(rtc::Point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector::Normalize(n)));
            }
        }
    }
}

SCENARIO("Computing the normal on a translated sphere", "[light and shading]")
{
    GIVEN("s <- sphere() and set_transform(s, translation(0, 1, 0))")
    {
        WHEN("n <- normal_at(s, point(0, 1.70711, -0.70711)")
        {
            THEN("n = vector(0, 0.70711, -0.70711)")
            {
                rtc::Sphere s(rtc::Matrix44::Translation(0, 1, 0));
                rtc::Vector n = s.NormalAt(rtc::Point(0, 1.70711, -0.70711));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector(0, 0.70711, -0.70711)));
            }
        }
    }
}

SCENARIO("Computing the normal on a transformed sphere", "[light and shading]")
{
    GIVEN("s <- sphere() and m <- scaling(1, 0.5, 1) * rotation_z(pi / 5.0) and set_transform(s, m)")
    {
        WHEN("n <- normal_at(s, point(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0)")
        {
            THEN("n = vector(0, 0.97014, -0.24254)")
            {
                rtc::Sphere s(rtc::Matrix44::Multiply(rtc::Matrix44::Scaling(1, 0.5, 1), rtc::Matrix44::RotationZ(rtc::kPi / 5.0)));
                rtc::Vector n = s.NormalAt(rtc::Point(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));

                REQUIRE(rtc::Tuple::Equal(n, rtc::Vector(0, 0.97014, -0.24254)));
            }
        }
    }
}

SCENARIO("Reflecting a vector approaching at 45 degrees", "[light and shading]")
{
    GIVEN("v <- vector(1, -1, 0) and n <- vector(0, 1, 0)")
    {
        WHEN("r <- reflect(v, n)")
        {
            THEN("r = vector(1, 1, 0)")
            {
                rtc::Vector v(1, -1, 0);
                rtc::Vector n(0, 1, 0);
                rtc::Vector r = rtc::Vector::Reflect(v, n);

                REQUIRE(rtc::Tuple::Equal(r, rtc::Vector(1, 1, 0)));
            }
        }
    }
}

SCENARIO("Reflecting a vector off a slanted surface", "[light and shading]")
{
    GIVEN("v <- vector(0, -1, 0) and n <- vector(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0)")
    {
        WHEN("r <- reflect(v, n)")
        {
            THEN("r = vector(1, 0, 0)")
            {
                rtc::Vector v(0, -1, 0);
                rtc::Vector n(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0);
                rtc::Vector r = rtc::Vector::Reflect(v, n);

                REQUIRE(rtc::Tuple::Equal(r, rtc::Vector(1, 0, 0)));
            }
        }
    }
}

SCENARIO("A point light has a position and intensity", "[light and shading]")
{
    GIVEN("intensity <- color(1, 1, 1) and position <- point(0, 0, 0)")
    {
        WHEN("light <- point_light(position, intensity)")
        {
            THEN("light.position = position and light.intensity = intensity")
            {
                rtc::Color      intensity(1, 1, 1);
                rtc::Point      position(0, 0, 0);
                rtc::PointLight light(position, intensity);

                REQUIRE(rtc::Tuple::Equal(light.GetPosition(), position));
                REQUIRE(rtc::Tuple::Equal(light.GetIntensity(), intensity));
            }
        }
    }
}

SCENARIO("The default material", "[light and shading]")
{
    GIVEN("m <- material")
    {
        THEN("m.color = color(1, 1, 1) and m.ambient = 0.1 and m.diffuse = 0.9 and m.specular = 0.9 and m.shininess = 200")
        {
            rtc::Material m;

            REQUIRE(rtc::Color::Equal(m.GetColor(), rtc::Color(1, 1, 1)));
            REQUIRE(rtc::Equal(m.GetAmbient(), 0.1));
            REQUIRE(rtc::Equal(m.GetDiffuse(), 0.9));
            REQUIRE(rtc::Equal(m.GetSpecular(), 0.9));
            REQUIRE(rtc::Equal(m.GetShininess(), 200));
        }
    }
}

SCENARIO("A sphere has a default material", "[light and shading]")
{
    GIVEN("s <- sphere")
    {
        WHEN("m <- s.material")
        {
            THEN("m = material()")
            {
                rtc::Sphere   s;
                rtc::Material m = s.GetMaterial();

                REQUIRE(rtc::Material::Equal(m, rtc::Material()));
            }
        }
    }
}

SCENARIO("A sphere may be assigned a material", "[light and shading]")
{
    GIVEN("s <- sphere and m <- material and m.ambient <- 1")
    {
        WHEN("s.material <- m")
        {
            THEN("s.material = m")
            {
                rtc::Sphere   s;
                rtc::Material m;

                m.SetAmbient(1);
                s.SetMaterial(m);

                REQUIRE(rtc::Material::Equal(s.GetMaterial(), m));
            }
        }
    }
}

SCENARIO("Lighting with the eye between the light and the surface", "[light and shading]")
{
    GIVEN("m <- material and position <- point(0, 0, 0) and eyev <- vector(0, 0, -1) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 0, -10), color(1, 1, 1)")
    {
        WHEN("result <- lighting(m, light, position, eyev, normalv)")
        {
            THEN("result = color(1.9, 1.9, 1.9)")
            {
                rtc::Material   m;
                rtc::Point      position(0, 0, 0);
                rtc::Vector     eyev(0, 0, -1);
                rtc::Vector     normalv(0, 0, -1);
                rtc::PointLight light(rtc::Point(0, 0, -10), rtc::Color(1, 1, 1));

                auto result = rtc::Phong::Lighting(m, light, position, eyev, normalv);

                REQUIRE(rtc::Color::Equal(result, rtc::Color(1.9, 1.9, 1.9)));
            }
        }
    }
}

SCENARIO("Lighting with the eye between light and surface, eye offset 45 degrees", "[light and shading]")
{
    GIVEN("m <- material and position <- point(0, 0, 0) and eyev <- vector(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 0, -10), color(1, 1, 1)")
    {
        WHEN("result <- lighting(m, light, position, eyev, normalv)")
        {
            THEN("result = color(1.0, 1.0, 1.0)")
            {
                rtc::Material   m;
                rtc::Point      position(0, 0, 0);
                rtc::Vector     eyev(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
                rtc::Vector     normalv(0, 0, -1);
                rtc::PointLight light(rtc::Point(0, 0, -10), rtc::Color(1, 1, 1));

                auto result = rtc::Phong::Lighting(m, light, position, eyev, normalv);

                REQUIRE(rtc::Color::Equal(result, rtc::Color(1.0, 1.0, 1.0)));
            }
        }
    }
}

SCENARIO("Lighting with eye opposite surface, light offset 45 degrees", "[light and shading]")
{
    GIVEN("m <- material and position <- point(0, 0, 0) and eyev <- vector(0, 0, -1) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 10, -10), color(1, 1, 1)")
    {
        WHEN("result <- lighting(m, light, position, eyev, normalv)")
        {
            THEN("result = color(0.7364, 0.7364, 0.7364)")
            {
                rtc::Material   m;
                rtc::Point      position(0, 0, 0);
                rtc::Vector     eyev(0, 0, -1);
                rtc::Vector     normalv(0, 0, -1);
                rtc::PointLight light(rtc::Point(0, 10, -10), rtc::Color(1, 1, 1));

                auto result = rtc::Phong::Lighting(m, light, position, eyev, normalv);

                REQUIRE(rtc::Color::Equal(result, rtc::Color(0.7364, 0.7364, 0.7364)));
            }
        }
    }
}

SCENARIO("Lighting with eye in the path of the reflection vector", "[light and shading]")
{
    GIVEN("m <- material and position <- point(0, 0, 0) and eyev <- vector(0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 10, -10), color(1, 1, 1)")
    {
        WHEN("result <- lighting(m, light, position, eyev, normalv)")
        {
            THEN("result = color(1.6364, 1.6364, 1.6364)")
            {
                rtc::Material   m;
                rtc::Point      position(0, 0, 0);
                rtc::Vector     eyev(0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
                rtc::Vector     normalv(0, 0, -1);
                rtc::PointLight light(rtc::Point(0, 10, -10), rtc::Color(1, 1, 1));

                auto result = rtc::Phong::Lighting(m, light, position, eyev, normalv);

                REQUIRE(rtc::Color::Equal(result, rtc::Color(1.6364, 1.6364, 1.6364)));
            }
        }
    }
}

SCENARIO("Lighting with the light behind the surface", "[light and shading]")
{
    GIVEN("m <- material and position <- point(0, 0, 0) and eyev <- vector(0, 0, -1) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 0, 10), color(1, 1, 1)")
    {
        WHEN("result <- lighting(m, light, position, eyev, normalv)")
        {
            THEN("result = color(0.1, 0.1, 0.1)")
            {
                rtc::Material   m;
                rtc::Point      position(0, 0, 0);
                rtc::Vector     eyev(0, 0, -1);
                rtc::Vector     normalv(0, 0, -1);
                rtc::PointLight light(rtc::Point(0, 0, 10), rtc::Color(1, 1, 1));

                auto result = rtc::Phong::Lighting(m, light, position, eyev, normalv);

                REQUIRE(rtc::Color::Equal(result, rtc::Color(0.1, 0.1, 0.1)));
            }
        }
    }
}
