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

#include "double_util.h"
#include "intersect.h"
#include "matrix44.h"
#include "point.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"

SCENARIO("Creating and querying a ray", "[ray]")
{
    GIVEN("origin <- point(1, 2, 3) and direction <- vector(4, 5, 6)")
    {
        WHEN("r <- ray(origin, direction)")
        {
            THEN("r.origin = origin and r.direction = direction")
            {
                rtc::Point  origin(1, 2, 3);
                rtc::Vector direction(4, 5, 6);
                rtc::Ray    r(origin, direction);

                REQUIRE(rtc::Tuple::Equal(r.GetOrigin(), origin));
                REQUIRE(rtc::Tuple::Equal(r.GetDirection(), direction));
            }
        }
    }
}

SCENARIO("Computing a point from a distance", "[ray]")
{
    GIVEN("r <- ray(point(2, 3, 4), vector(1, 0, 0))")
    {
        WHEN("Computing a position)")
        {
            THEN("position(r, 0) = point(2, 3, 4) and position(r, 1) = point(3, 3, 4) and position(r, -1) = point(1, 3, 4) and position(r, 2.5) = point(4.5, 3, 4)")
            {
                rtc::Ray r(rtc::Point(2, 3, 4), rtc::Vector(1, 0, 0));

                REQUIRE(rtc::Tuple::Equal(r.GetPosition(0), rtc::Point(2, 3, 4)));
                REQUIRE(rtc::Tuple::Equal(r.GetPosition(1), rtc::Point(3, 3, 4)));
                REQUIRE(rtc::Tuple::Equal(r.GetPosition(-1), rtc::Point(1, 3, 4)));
                REQUIRE(rtc::Tuple::Equal(r.GetPosition(2.5), rtc::Point(4.5, 3, 4)));
            }
        }
    }
}

SCENARIO("A ray intersects a sphere at two points", "[ray]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and s <- sphere()")
    {
        WHEN("xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].t = 4.0 and xs[1].t = 6.0")
            {
                rtc::Ray       r(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s;
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 2);
                REQUIRE(rtc::Equal(xs.GetValue(0).t, 4.0));
                REQUIRE(rtc::Equal(xs.GetValue(1).t, 6.0));
            }
        }
    }
}

SCENARIO("A ray intersects a sphere at a tangent", "[ray]")
{
    GIVEN("r <- ray(point(0, 1, -5), vector(0, 0, 1)) and s <- sphere()")
    {
        WHEN("xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].t = 5.0 and xs[1].t = 5.0")
            {
                rtc::Ray       r(rtc::Point(0, 1, -5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s;
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 2);
                REQUIRE(rtc::Equal(xs.GetValue(0).t, 5.0));
                REQUIRE(rtc::Equal(xs.GetValue(1).t, 5.0));
            }
        }
    }
}

SCENARIO("A ray misses a sphere", "[ray]")
{
    GIVEN("r <- ray(point(0, 2, -5), vector(0, 0, 1)) and s <- sphere()")
    {
        WHEN("xs <- intersect(s, r)")
        {
            THEN("xs.count = 0")
            {
                rtc::Ray       r(rtc::Point(0, 2, -5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s;
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 0);
            }
        }
    }
}

SCENARIO("A ray originates inside a sphere", "[ray]")
{
    GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1)) and s <- sphere()")
    {
        WHEN("xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].t = -1.0 and xs[1].t = 1.0")
            {
                rtc::Ray       r(rtc::Point(0, 0, 0), rtc::Vector(0, 0, 1));
                rtc::Sphere    s;
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 2);
                REQUIRE(rtc::Equal(xs.GetValue(0).t, -1.0));
                REQUIRE(rtc::Equal(xs.GetValue(1).t, 1.0));
            }
        }
    }
}

SCENARIO("A sphere is behind a ray", "[ray]")
{
    GIVEN("r <- ray(point(0, 0, 5), vector(0, 0, 1)) and s <- sphere()")
    {
        WHEN("xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].t = -6.0 and xs[1].t = -4.0")
            {
                rtc::Ray       r(rtc::Point(0, 0, 5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s;
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 2);
                REQUIRE(rtc::Equal(xs.GetValue(0).t, -6.0));
                REQUIRE(rtc::Equal(xs.GetValue(1).t, -4.0));
            }
        }
    }
}

SCENARIO("An intersection encapsulates t and object", "[ray]")
{
    GIVEN("s <- sphere()")
    {
        WHEN("i <- intersection(3.5, s)")
        {
            THEN("i.t = 3.5 and i.object = s")
            {
                rtc::Sphere                  s;
                rtc::Intersect::Intersection i{ 3.5, &s };

                REQUIRE(rtc::Equal(i.t, 3.5));
                REQUIRE(i.object == &s);
            }
        }
    }
}

SCENARIO("Aggregating intersections", "[ray]")
{
    GIVEN("s <- sphere() and i1 <- intersection(1, s) and i2 <- intersection(2, s)")
    {
        WHEN("xs <- intersections(i1, i2)")
        {
            THEN("xs.count = 2 and xs[0].t = 1 and xs[1].t = 2")
            {
                rtc::Sphere                   s;
                rtc::Intersect::Intersection  i1{ 1, &s };
                rtc::Intersect::Intersection  i2{ 2, &s };
                rtc::Intersect::Intersections xs{ i1, i2 };

                REQUIRE(xs.size() == 2);
                REQUIRE(rtc::Equal(xs[0].t, 1));
                REQUIRE(rtc::Equal(xs[1].t, 2));
            }
        }
    }
}

SCENARIO("Intersect sets the object on the intersection", "[ray]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and s <- sphere()")
    {
        WHEN("xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].object = s and xs[1].object = s")
            {
                rtc::Ray       r(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s;
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 2);
                REQUIRE(xs.GetValue(0).object == &s);
                REQUIRE(xs.GetValue(1).object == &s);
            }
        }
    }
}

SCENARIO("The hit, when all intersections have positive t", "[ray]")
{
    GIVEN("s <- sphere() and i1 <- intersection(1, s) and i2 <- intersection(2, s) and xs <- intersections(i2, i1)")
    {
        WHEN("i <- hit(xs)")
        {
            THEN("i = i1")
            {
                rtc::Sphere                   s;
                rtc::Intersect::Intersection  i1{ 1, &s };
                rtc::Intersect::Intersection  i2{ 2, &s };
                rtc::Intersect::Intersections xs{ i2, i1 };

                rtc::Intersect::Sort(xs);
                auto i = rtc::Intersect::Hit(xs);

                REQUIRE(i != nullptr);
                if (i != nullptr)
                {
                    REQUIRE(rtc::Equal(i->t, i1.t));
                    REQUIRE(i->object == i1.object);
                }
            }
        }
    }
}

SCENARIO("The hit, when some intersections have negative t", "[ray]")
{
    GIVEN("s <- sphere() and i1 <- intersection(-1, s) and i2 <- intersection(1, s) and xs <- intersections(i2, i1)")
    {
        WHEN("i <- hit(xs)")
        {
            THEN("i = i2")
            {
                rtc::Sphere                   s;
                rtc::Intersect::Intersection  i1{ -1, &s };
                rtc::Intersect::Intersection  i2{ 1, &s };
                rtc::Intersect::Intersections xs{ i2, i1 };

                auto i = rtc::Intersect::Hit(xs);

                REQUIRE(i != nullptr);
                if (i != nullptr)
                {
                    REQUIRE(rtc::Equal(i->t, i2.t));
                    REQUIRE(i->object == i2.object);
                }
            }
        }
    }
}

SCENARIO("The hit, when all intersections have negative t", "[ray]")
{
    GIVEN("s <- sphere() and i1 <- intersection(-2, s) and i2 <- intersection(-1, s) and xs <- intersections(i2, i1)")
    {
        WHEN("i <- hit(xs)")
        {
            THEN("i = nothing")
            {
                rtc::Sphere                   s;
                rtc::Intersect::Intersection  i1{ -2, &s };
                rtc::Intersect::Intersection  i2{ -1, &s };
                rtc::Intersect::Intersections xs{ i2, i1 };

                auto i = rtc::Intersect::Hit(xs);

                REQUIRE(i == nullptr);
            }
        }
    }
}

SCENARIO("Translating a ray", "[ray]")
{
    GIVEN("r <- ray(point(1, 2, 3), vector(0, 1, 0)) and m <- translation(3, 4, 5)")
    {
        WHEN("r2 <- transform(r, m)")
        {
            THEN("r2.origin = point(4, 6, 8) and r2.direcetion = vector(0, 1, 0)")
            {
                rtc::Ray      r(rtc::Point(1, 2, 3), rtc::Vector(0, 1, 0));
                rtc::Matrix44 m = rtc::Matrix44::Translation(3, 4, 5);

                rtc::Ray r2 = rtc::Matrix44::Transform(r, m);

                REQUIRE(rtc::Point::Equal(r2.GetOrigin(), rtc::Point(4, 6, 8)));
                REQUIRE(rtc::Vector::Equal(r2.GetDirection(), rtc::Vector(0, 1, 0)));
            }
        }
    }
}

SCENARIO("Scaling a ray", "[ray]")
{
    GIVEN("r <- ray(point(1, 2, 3), vector(0, 1, 0)) and m <- scaling(2, 3, 4)")
    {
        WHEN("r2 <- transform(r, m)")
        {
            THEN("r2.origin = point(2, 6, 12) and r2.direcetion = vector(0, 3, 0)")
            {
                rtc::Ray      r(rtc::Point(1, 2, 3), rtc::Vector(0, 1, 0));
                rtc::Matrix44 m = rtc::Matrix44::Scaling(2, 3, 4);

                rtc::Ray r2 = rtc::Matrix44::Transform(r, m);

                REQUIRE(rtc::Point::Equal(r2.GetOrigin(), rtc::Point(2, 6, 12)));
                REQUIRE(rtc::Vector::Equal(r2.GetDirection(), rtc::Vector(0, 3, 0)));
            }
        }
    }
}

SCENARIO("A sphere's default transformation", "[ray]")
{
    GIVEN("s <- sphere")
    {
        THEN("s.transform = identity_matrix")
        {
            rtc::Sphere s;

            REQUIRE(rtc::Matrix44::Equal(s.GetTransform(), rtc::Matrix44::Identity()));
        }
    }
}

SCENARIO("Changing a sphere's transformation", "[ray]")
{
    GIVEN("s <- sphere and t <- translation(2, 3, 4)")
    {
        WHEN("set_transform(s, t)")
        {
            THEN("s.transform = t")
            {
                rtc::Sphere   s;
                rtc::Matrix44 t = rtc::Matrix44::Translation(2, 3, 4);

                s.SetTransform(t);

                REQUIRE(rtc::Matrix44::Equal(s.GetTransform(), t));
            }
        }
    }
}

SCENARIO("Intersecting a scaled sphere with a ray", "[ray]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and s <- sphere")
    {
        WHEN("set_transform(s, scaling(2, 2, 2)) and xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].t = 3 and xs[1].t = 7")
            {
                rtc::Ray       r(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s(rtc::Matrix44::Scaling(2, 2, 2));
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 2);
                REQUIRE(rtc::Equal(xs.GetValue(0).t, 3));
                REQUIRE(rtc::Equal(xs.GetValue(1).t, 7));
            }
        }
    }
}

SCENARIO("Intersecting a translated sphere with a ray", "[ray]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and s <- sphere")
    {
        WHEN("set_transform(s, scaling(2, 2, 2)) and xs <- intersect(s, r)")
        {
            THEN("xs.count = 2 and xs[0].t = 3 and xs[1].t = 7")
            {
                rtc::Ray       r(rtc::Point(0, 0, -5), rtc::Vector(0, 0, 1));
                rtc::Sphere    s(rtc::Matrix44::Translation(5, 0, 0));
                rtc::Intersect xs(&s, r);

                REQUIRE(xs.GetCount() == 0);
            }
        }
    }
}
