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

#include "double_util.h"
#include "intersections.h"
#include "material.h"
#include "matrix44.h"
#include "plane.h"
#include "point.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

#include <cmath>

class TestShape : public rtc::Shape
{
public:
    static const rtc::Ray& GetSavedRay() { return saved_ray_; }

private:
    virtual void LocalIntersect(const rtc::Ray& local_ray, rtc::Intersections::Values&) const override { saved_ray_ = local_ray; }

    virtual rtc::Vector LocalNormalAt(const rtc::Point& local_point) const override { return rtc::Vector{ local_point }; }

private:
    static rtc::Ray saved_ray_;
};

rtc::Ray TestShape::saved_ray_{ rtc::Point{}, rtc::Vector{} };

SCENARIO("The default shape transformation", "[refactoring shapes]")
{
    GIVEN("s <- test_shape()")
    {
        const auto s = TestShape{};

        THEN("s.transform = identity_matrix")
        {
            REQUIRE(rtc::Matrix44::Equal(s.GetTransform(), rtc::Matrix44::Identity()));
        }
    }
}

SCENARIO("Assigining a transformation", "[refactoring shapes]")
{
    GIVEN("s <- test_shape()")
    {
        auto s = TestShape{};

        WHEN("set_transform(s, translation(2, 3, 4))")
        {
            s.SetTransform(rtc::Matrix44::Translation(2.0, 3.0, 4.0));

            THEN("s.transform = identity_matrix")
            {
                REQUIRE(rtc::Matrix44::Equal(s.GetTransform(), rtc::Matrix44::Translation(2.0, 3.0, 4.0)));
            }
        }
    }
}

SCENARIO("The default shape material", "[refactoring shapes]")
{
    GIVEN("s <- test_shape()")
    {
        const auto s = TestShape{};

        WHEN("m <- s.material")
        {
            const auto m = s.GetMaterial();

            THEN("m = material()")
            {
                REQUIRE(rtc::Material::Equal(m, rtc::Material{}));
            }
        }
    }
}

SCENARIO("Assigining a material", "[refactoring shapes]")
{
    GIVEN("s <- test_shape() and m <- material() and m.ambient <- 1")
    {
        auto s = TestShape{};
        auto m = rtc::Material{};
        m.SetAmbient(1.0);

        WHEN("s.material <- m")
        {
            s.SetMaterial(m);

            THEN("s.material = m")
            {
                REQUIRE(rtc::Material::Equal(s.GetMaterial(), m));
            }
        }
    }
}

SCENARIO("Intersecting a scaled shape with a ray", "[refactoring shapes]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and s <- test_shape()")
    {
        const auto r = rtc::Ray{ rtc::Point{ 0.0, 0.0, -5.0 }, rtc::Vector{ 0.0, 0.0, 1.0 } };
        auto       s = TestShape{};

        WHEN("set_transform(s, scaling(2, 2, 2)) and xs <- intersect(s, r)")
        {
            s.SetTransform(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));

            const auto xs = s.Intersect( r);

            THEN("s.saved_ray.origin = point(0, 0, -2.5) and s.saved_ray.direction = vector(0, 0, 0.5)")
            {
                const auto& saved_ray = s.GetSavedRay();

                REQUIRE(rtc::Point::Equal(saved_ray.GetOrigin(), rtc::Point{ 0.0, 0.0, -2.5 }));
                REQUIRE(rtc::Vector::Equal(saved_ray.GetDirection(), rtc::Vector{ 0.0, 0.0, 0.5 }));
            }
        }
    }
}

SCENARIO("Intersecting a translated shape with a ray", "[refactoring shapes]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)) and s <- test_shape()")
    {
        const auto r = rtc::Ray{ rtc::Point{ 0.0, 0.0, -5.0 }, rtc::Vector{ 0.0, 0.0, 1.0 } };
        auto       s = TestShape{};

        WHEN("set_transform(s, translation(5, 0, 0)) and xs <- intersect(s, r)")
        {
            s.SetTransform(rtc::Matrix44::Translation(5.0, 0.0, 0.0));

            const auto xs = s.Intersect(r);

            THEN("s.saved_ray.origin = point(-5, 0, -5) and s.saved_ray.direction = vector(0, 0, 1)")
            {
                const auto& saved_ray = s.GetSavedRay();

                REQUIRE(rtc::Point::Equal(saved_ray.GetOrigin(), rtc::Point{ -5.0, 0.0, -5.0 }));
                REQUIRE(rtc::Vector::Equal(saved_ray.GetDirection(), rtc::Vector{ 0.0, 0.0, 1.0 }));
            }
        }
    }
}

SCENARIO("Computing the normal on a translated shape", "[refactoring shapes]")
{
    GIVEN("s <- test_shape()")
    {
        auto s = TestShape{};

        WHEN("set_transform(s, translation(0, 1, 0)) and n <- normal_at(s, point(0, 1.70711, -0.70711))")
        {
            s.SetTransform(rtc::Matrix44::Translation(0.0, 1.0, 0.0));

            const auto n = s.NormalAt(rtc::Point{ 0.0, 1.70711, -0.70711 });

            THEN("n = vector(0, 0.70711, -0.70711)")
            {
                REQUIRE(rtc::Vector::Equal(n, rtc::Vector{ 0.0, 0.70711, -0.70711 }));
            }
        }
    }
}

SCENARIO("Computing the normal on a transformed shape", "[refactoring shapes]")
{
    GIVEN("s <- test_shape() and m <- scaling(1, 0.5, 1) * rotation_z(pi/5)")
    {
        auto       s = TestShape{};
        const auto m = rtc::Matrix44::Multiply(rtc::Matrix44::Scaling(1.0, 0.5, 1.0), rtc::Matrix44::RotationZ(rtc::kPi / 5.0));

        WHEN("set_transform(s, m) and n <- normal_at(s, point(0, sqrt(2)/2, -sqrt(2)/2))")
        {
            s.SetTransform(m);

            const auto n = s.NormalAt(rtc::Point{ 0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0 });

            THEN("n = vector(0, 0.97014, -0.24254)")
            {
                REQUIRE(rtc::Vector::Equal(n, rtc::Vector{ 0.0, 0.97014, -0.24254 }));
            }
        }
    }
}

SCENARIO("The normal of a plane is constant everywhere", "[refactoring shapes]")
{
    GIVEN("p <- plane()")
    {
        const auto p = rtc::Plane::Create();

        WHEN("n1 <- local_normal_at(p, point(0, 0, 0)) and n2 <- local_normal_at(p, point(10, 0, -10)) and n3 <- local_normal_at(p, point(-5, 0, 150))")
        {
            // NOTE: Test is using the public NormalAt, instead of the private LocalNormalAt, which produces the same result with the default transform.
            const auto n1 = p->NormalAt(rtc::Point{ 0.0, 0.0, 0.0 });
            const auto n2 = p->NormalAt(rtc::Point{ 10.0, 0.0, -10.0 });
            const auto n3 = p->NormalAt(rtc::Point{ -5.0, 0.0, 150.0 });

            THEN("n1 = vector(0, 1, 0) and n2 = vector(0, 1, 0) and n3 = vector(0, 1, 0)")
            {
                REQUIRE(rtc::Vector::Equal(n1, rtc::Vector{ 0.0, 1.0, 0.0 }));
                REQUIRE(rtc::Vector::Equal(n2, rtc::Vector{ 0.0, 1.0, 0.0 }));
                REQUIRE(rtc::Vector::Equal(n3, rtc::Vector{ 0.0, 1.0, 0.0 }));
            }
        }
    }
}

SCENARIO("Intersect with a ray parallel to the plane", "[refactoring shapes]")
{
    GIVEN("p <- plane() and r <- ray(point(0, 10, 0), vector(0, 0, 1))")
    {
        const auto p = rtc::Plane::Create();
        const auto r = rtc::Ray(rtc::Point{ 0.0, 10.0, 1.0 }, rtc::Vector{ 0.0, 0.0, 1.0 });

        WHEN("xs <- local_intersect(p, r)")
        {
            // NOTE: Test is using the public Intersect, instead of the private LocalIntersect, which produces the same result with the default transform.
            const auto xs = p->Intersect(r);

            THEN("xs is empty")
            {
                REQUIRE(xs.GetCount() == 0);
            }
        }
    }
}

SCENARIO("Intersect with a coplanar ray", "[refactoring shapes]")
{
    GIVEN("p <- plane() and r <- ray(point(0, 0, 0), vector(0, 0, 1))")
    {
        const auto p = rtc::Plane::Create();
        const auto r = rtc::Ray(rtc::Point{ 0.0, 0.0, 1.0 }, rtc::Vector{ 0.0, 0.0, 1.0 });

        WHEN("xs <- local_intersect(p, r)")
        {
            // NOTE: Test is using the public Intersect, instead of the private LocalIntersect, which produces the same result with the default transform.
            const auto xs = p->Intersect(r);

            THEN("xs is empty")
            {
                REQUIRE(xs.GetCount() == 0);
            }
        }
    }
}

SCENARIO("A ray intersecting a plane from above", "[refactoring shapes]")
{
    GIVEN("p <- plane() and r <- ray(point(0, 1, 0), vector(0, -1, 0))")
    {
        const auto p = rtc::Plane::Create();
        const auto r = rtc::Ray(rtc::Point{ 0.0, 1.0, 0.0 }, rtc::Vector{ 0.0, -1.0, 0.0 });

        WHEN("xs <- local_intersect(p, r)")
        {
            // NOTE: Test is using the public Intersect, instead of the private LocalIntersect, which produces the same result with the default transform.
            const auto xs = p->Intersect(r);

            THEN("xs.count = 1 and xs[0].t = 1 and xs[0].object = p")
            {
                REQUIRE(xs.GetCount() == 1);
                REQUIRE(rtc::Equal(xs.GetValue(0).GetT(), 1.0));
                REQUIRE(xs.GetValue(0).GetObject() == p);
            }
        }
    }
}

SCENARIO("A ray intersecting a plane from below", "[refactoring shapes]")
{
    GIVEN("p <- plane() and r <- ray(point(0, -1, 0), vector(0, 1, 0))")
    {
        const auto p = rtc::Plane::Create();
        const auto r = rtc::Ray(rtc::Point{ 0.0, -1.0, 0.0 }, rtc::Vector{ 0.0, 1.0, 0.0 });

        WHEN("xs <- local_intersect(p, r)")
        {
            // NOTE: Test is using the public Intersect, instead of the private LocalIntersect, which produces the same result with the default transform.
            const auto xs = p->Intersect(r);

            THEN("xs.count = 1 and xs[0].t = 1 and xs[0].object = p")
            {
                REQUIRE(xs.GetCount() == 1);
                REQUIRE(rtc::Equal(xs.GetValue(0).GetT(), 1.0));
                REQUIRE(xs.GetValue(0).GetObject() == p);
            }
        }
    }
}
