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

#include "checkers_pattern.h"
#include "color.h"
#include "gradient_pattern.h"
#include "material.h"
#include "matrix44.h"
#include "phong.h"
#include "point.h"
#include "point_light.h"
#include "ring_pattern.h"
#include "sphere.h"
#include "stripe_pattern.h"
#include "vector.h"

const rtc::Color kBlack{ 0.0, 0.0, 0.0 };
const rtc::Color kWhite{ 1.0, 1.0, 1.0 };

class TestPattern : public rtc::Pattern
{
  public:
      virtual rtc::Color PatternAt(const rtc::Point& point) const { return rtc::Color{ point.GetX(), point.GetY(), point.GetZ() }; }
};

SCENARIO("Creating a stripe pattern", "[patterns]")
{
    GIVEN("pattern <- stripe_pattern(white, black)")
    {
        const auto pattern = rtc::StripePattern::Create(kWhite, kBlack);

        THEN("pattern.a = white and pattern.b = black")
        {
            REQUIRE(rtc::Color::Equal(pattern->GetA(), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->GetB(), kBlack));
        }
    }
}

SCENARIO("A stripe pattern is constant in y", "[patterns]")
{
    GIVEN("pattern <- stripe_pattern(white, black)")
    {
        const auto pattern = rtc::StripePattern::Create(kWhite, kBlack);

        THEN("stripe_at(pattern, point(0, 0, 0)) = white and stripe_at(pattern, point(0, 1, 0)) = white and stripe_at(pattern, point(0, 2, 0)) = white")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 1.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 2.0, 0.0 }), kWhite));
        }
    }
}

SCENARIO("A stripe pattern is constant in z", "[patterns]")
{
    GIVEN("pattern <- stripe_pattern(white, black)")
    {
        const auto pattern = rtc::StripePattern::Create(kWhite, kBlack);

        THEN("stripe_at(pattern, point(0, 0, 0)) = white and stripe_at(pattern, point(0, 0, 1)) = white and stripe_at(pattern, point(0, 0, 2)) = white")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 1.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 2.0 }), kWhite));
        }
    }
}

SCENARIO("A stripe pattern alternates in x", "[patterns]")
{
    GIVEN("pattern <- stripe_pattern(white, black)")
    {
        const auto pattern = rtc::StripePattern::Create(kWhite, kBlack);

        THEN("stripe_at(pattern, point(0, 0, 0)) = white and stripe_at(pattern, point(0.9, 0, 0)) = white and stripe_at(pattern, point(1, 0, 0)) = white and stripe_at(pattern, point(-0.1, 0, 0)) = white and stripe_at(pattern, point(-1, 0, 0)) = white and stripe_at(pattern, point(-1.1, 0, 0)) = white")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.9, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 1.0, 0.0, 0.0 }), kBlack));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ -0.1, 0.0, 0.0 }), kBlack));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ -1.0, 0.0, 0.0 }), kBlack));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ -1.1, 0.0, 0.0 }), kWhite));
        }
    }
}

SCENARIO("Lighting with a pattern applied", "[patterns]")
{
    GIVEN("m.pattern <- stripe_pattern(color(1, 1, 1), color(0, 0, 0)) and m.ambient = 1 and m.diffuse <- 0 and m.specular <- 0 and eyev <- vector(0, 0, -1) and normalv <- vector(0, 0, -1) and light <- point_light(point(0, 0, -10), color(1, 1, 1))")
    {
        const auto m       = rtc::Material{ rtc::StripePattern::Create(kWhite, kBlack), 1.0, 0.0, 0.0, rtc::Material::GetDefaultShininess() };
        const auto eyev    = rtc::Vector{ 0.0, 0.0, -1.0 };
        const auto normalv = rtc::Vector{ 0.0, 0.0, -1.0 };
        const auto light   = rtc::PointLight{ rtc::Point{ 0.0, 0.0, -10.0 }, rtc::Color{ 1.0, 1.0, 1.0 } };

        WHEN("c1 <- lighting(m, light, point(0.9, 0, 0), eyev, normalv, false) and c2 <- lighting(m, light, point(1.1, 0, 0), eyev, normalv, false)")
        {
            const auto c1 = rtc::Phong::Lighting(m, rtc::Matrix44::Identity(), light, rtc::Point(0.9, 0.0, 0.0), eyev, normalv, false);
            const auto c2 = rtc::Phong::Lighting(m, rtc::Matrix44::Identity(), light, rtc::Point(1.1, 0.0, 0.0), eyev, normalv, false);

            THEN("c1 = color(1, 1, 1) and c2 = color(0, 0, 0)")
            {
                REQUIRE(rtc::Color::Equal(c1, rtc::Color{ 1.0, 1.0, 1.0 }));
                REQUIRE(rtc::Color::Equal(c2, rtc::Color{ 0.0, 0.0, 0.0 }));
            }
        }
    }
}

SCENARIO("Stripes with an object transformation", "[patterns]")
{
    GIVEN("object <- sphere() and set_transform(object, scaling(2, 2, 2)) and pattern <- stripe_pattern(white, black)")
    {
        const auto object  = rtc::Sphere::Create(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));
        const auto pattern = rtc::StripePattern::Create(kWhite, kBlack);

        WHEN("c <- stripe_at_object(pattern, object, point(1.5, 0, 0))")
        {
            const auto c = pattern->PatternAtObject(object->GetInverseTransform(), rtc::Point{1.5, 0.0, 0.0});

            THEN("c = white")
            {
                REQUIRE(rtc::Color::Equal(c, kWhite));
            }
        }
    }
}

SCENARIO("Stripes with a pattern transformation", "[patterns]")
{
    GIVEN("object <- sphere() and pattern <- stripe_pattern(white, black) and set_pattern_transform(pattern, scaling(2, 2, 2))")
    {
        const auto object  = rtc::Sphere::Create();
        auto       pattern = rtc::StripePattern::Create(kWhite, kBlack);

        pattern->SetTransform(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));

        WHEN("c <- stripe_at_object(pattern, object, point(1.5, 0, 0))")
        {
            const auto c = pattern->PatternAtObject(object->GetInverseTransform(), rtc::Point{1.5, 0.0, 0.0});

            THEN("c = white")
            {
                REQUIRE(rtc::Color::Equal(c, kWhite));
            }
        }
    }
}

SCENARIO("Stripes with both an object and a pattern transformation", "[patterns]")
{
    GIVEN("object <- sphere() and set_transform(object, scaling(2, 2, 2)) and pattern <- stripe_pattern(white, black) and set_pattern_transform(pattern, translation(0.5, 0, 0))")
    {
        const auto object  = rtc::Sphere::Create(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));
        const auto pattern = rtc::StripePattern::Create(kWhite, kBlack, rtc::Matrix44::Translation(0.5, 0.0, 0.0));

        WHEN("c <- stripe_at_object(pattern, object, point(1.5, 0, 0))")
        {
            const auto c = pattern->PatternAtObject(object->GetInverseTransform(), rtc::Point{ 2.5, 0.0, 0.0 });

            THEN("c = white")
            {
                REQUIRE(rtc::Color::Equal(c, kWhite));
            }
        }
    }
}

SCENARIO("The default pattern transformation", "[patterns]")
{
    GIVEN("pattern <- test_pattern()")
    {
        const auto pattern = TestPattern{};

        THEN("pattern.transform = identity_matrix")
        {
            REQUIRE(rtc::Matrix44::Equal(pattern.GetTransform(), rtc::Matrix44::Identity()));
        }
    }
}

SCENARIO("Assigning a transformation", "[patterns]")
{
    GIVEN("pattern <- test_pattern()")
    {
        auto pattern = TestPattern{};

        WHEN("set_pattern_transform(pattern, translation(1, 2, 3)")
        {
            pattern.SetTransform(rtc::Matrix44::Translation(1.0, 2.0, 3.0));

            THEN("pattern.transform = translation(1, 2, 3)")
            {
                REQUIRE(rtc::Matrix44::Equal(pattern.GetTransform(), rtc::Matrix44::Translation(1.0, 2.0, 3.0)));
            }
        }
    }
}

SCENARIO("A pattern with an object transformation", "[patterns]")
{
    GIVEN("shape <- sphere() and set_transform(shape, scaling(2, 2, 2)) and pattern <- test_pattern()")
    {
        const auto shape   = rtc::Sphere::Create(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));
        const auto pattern = TestPattern{};

        WHEN("c <- pattern_at_shape(pattern, object, point(2, 3, 4))")
        {
            const auto c = pattern.PatternAtObject(shape->GetInverseTransform(), rtc::Point{ 2.0, 3.0, 4.0 });

            THEN("c = color(1, 1.5, 2)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color{ 1.0, 1.5, 2.0 }));
            }
        }
    }
}

SCENARIO("A pattern with a pattern transformation", "[patterns]")
{
    GIVEN("shape <- sphere() and pattern <- test_pattern() and set_pattern_transform(pattern, scaling(2, 2, 2))")
    {
        const auto shape   = rtc::Sphere::Create();
        auto       pattern = TestPattern{};

        pattern.SetTransform(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));

        WHEN("c <- pattern_at_shape(pattern, object, point(2, 3, 4))")
        {
            const auto c = pattern.PatternAtObject(shape->GetInverseTransform(), rtc::Point{ 2.0, 3.0, 4.0 });

            THEN("c = color(1, 1.5, 2)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color{ 1.0, 1.5, 2.0 }));
            }
        }
    }
}

SCENARIO("A pattern with both an object and a pattern transformation", "[patterns]")
{
    GIVEN("shape <- sphere() and set_transform(object, scaling(2, 2, 2)) and pattern <- stripe_pattern(white, black) and set_pattern_transform(pattern, translation(0.5, 1, 1.5))")
    {
        const auto shape   = rtc::Sphere::Create(rtc::Matrix44::Scaling(2.0, 2.0, 2.0));
        auto       pattern = TestPattern{};

        pattern.SetTransform(rtc::Matrix44::Translation(0.5, 1.0, 1.5));

        WHEN("c <- stripe_at_object(pattern, object, point(2.5, 3, 3.5))")
        {
            const auto c = pattern.PatternAtObject(shape->GetInverseTransform(), rtc::Point{ 2.5, 3.0, 3.5 });

            THEN("c = color(0.75, 0.5, 0.25)")
            {
                REQUIRE(rtc::Color::Equal(c, rtc::Color{ 0.75, 0.5, 0.25 }));
            }
        }
    }
}

SCENARIO("A gradient linearly interpolates between colors", "[patterns]")
{
    GIVEN("pattern <- gradient_pattern(white, black)")
    {
        const auto pattern = rtc::GradientPattern::Create(kWhite, kBlack);

        THEN("pattern_at(pattern, point(0, 0, 0)) = white and pattern_at(pattern, point(0.25, 0, 0)) = color(0.75, 0.75, 0.75) and pattern_at(pattern, point(0.5, 0, 0)) = color(0.5, 0.5, 0.5) and pattern_at(pattern, point(0.75, 0, 0)) = color(0.25, 0.25, 0.25)")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.25, 0.0, 0.0 }), rtc::Color{ 0.75, 0.75, 0.75 }));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.5, 0.0, 0.0 }), rtc::Color{ 0.5, 0.5, 0.5 }));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.75, 0.0, 0.0 }), rtc::Color{ 0.25, 0.25, 0.25 }));
        }
    }
}

SCENARIO("A ring should extend in both x and z", "[patterns]")
{
    GIVEN("pattern <- ring_pattern(white, black)")
    {
        const auto pattern = rtc::RingPattern::Create(kWhite, kBlack);

        THEN("pattern_at(pattern, point(0, 0, 0)) = white and pattern_at(pattern, point(1, 0, 0)) = black and pattern_at(pattern, point(0, 0, 1)) = black and pattern_at(pattern, point(0.708, 0, 0.708)) = black")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 1.0, 0.0, 0.0 }), kBlack));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 1.0 }), kBlack));
            // 0.708 is just slightly more than sqrt(2)/2.
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.708, 0.0, 0.708 }), kBlack));
        }
    }
}

SCENARIO("Checkers should repeat in x", "[patterns]")
{
    GIVEN("pattern <- checkers_pattern(white, black)")
    {
        const auto pattern = rtc::CheckersPattern::Create(kWhite, kBlack);

        THEN("pattern_at(pattern, point(0, 0, 0)) = white and pattern_at(pattern, point(0.99, 0, 0)) = white and pattern_at(pattern, point(1.01, 0, 0)) = black")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.99, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 1.01, 0.0, 0.0 }), kBlack));
        }
    }
}

SCENARIO("Checkers should repeat in y", "[patterns]")
{
    GIVEN("pattern <- checkers_pattern(white, black)")
    {
        const auto pattern = rtc::CheckersPattern::Create(kWhite, kBlack);

        THEN("pattern_at(pattern, point(0, 0, 0)) = white and pattern_at(pattern, point(0, 0.99, 0)) = white and pattern_at(pattern, point(0, 1.01, 0)) = black")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.99, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 1.01, 0.0 }), kBlack));
        }
    }
}

SCENARIO("Checkers should repeat in z", "[patterns]")
{
    GIVEN("pattern <- checkers_pattern(white, black)")
    {
        const auto pattern = rtc::CheckersPattern::Create(kWhite, kBlack);

        THEN("pattern_at(pattern, point(0, 0, 0)) = white and pattern_at(pattern, point(0, 0, 0.99)) = white and pattern_at(pattern, point(0, 0, 1.01)) = black")
        {
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.0 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 0.99 }), kWhite));
            REQUIRE(rtc::Color::Equal(pattern->PatternAt(rtc::Point{ 0.0, 0.0, 1.01 }), kBlack));
        }
    }
}
