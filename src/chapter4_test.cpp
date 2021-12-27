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
#include "matrix44.h"
#include "point.h"
#include "vector.h"

#include <cmath>

SCENARIO("Multiplying by a translation matrix", "[transform]")
{
    GIVEN("transform <- translation(5, -3, 2) and p <- point(-3, 4, 5)")
    {
        const auto transform = rtc::Matrix44::Translation(5.0, -3.0, 2.0);
        const auto p         = rtc::Point{ -3.0, 4.0, 5.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(2, 1, 7)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 2.0, 1.0, 7.0 }));
            }
        }
    }
}

SCENARIO("Multiplying by the inverse of a translation matrix", "[transform]")
{
    GIVEN("transform <- translation(5, -3, 2) and inv <- inverse(transform) and p <- point(-3, 4, 5)")
    {
        const auto transform = rtc::Matrix44::Translation(5.0, -3.0, 2.0);
        const auto inv       = rtc::Matrix44::Inverse(transform);
        const auto p         = rtc::Point{ -3.0, 4.0, 5.0 };

        WHEN("The inverse transform is applied to p")
        {
            THEN("inv * p = point(-8, 7, 3)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(inv, p), rtc::Point{ -8.0, 7.0, 3.0 }));
            }
        }
    }
}

SCENARIO("Translation does not affect vectors", "[transform]")
{
    GIVEN("transform <- translation(5, -3, 2) and v <- vector(5, -3, 2)")
    {
        const auto transform = rtc::Matrix44::Translation(5.0, -3.0, 2.0);
        const auto v         = rtc::Vector{ 5.0, -3.0, 2.0 };

        WHEN("The transform is applied to v")
        {
            THEN("transform * v = v")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, v), v));
            }
        }
    }
}

SCENARIO("A scaling matrix applied to a point", "[transform]")
{
    GIVEN("transform <- scaling(2, 3, 4) and p <- point(-4, 6, 8)")
    {
        const auto transform = rtc::Matrix44::Scaling(2.0, 3.0, 4.0);
        const auto p         = rtc::Point{ -4.0, 6.0, 8.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(-8, 18, 32)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ -8.0, 18.0, 32.0 }));
            }
        }
    }
}

SCENARIO("A scaling matrix applied to a vector", "[transform]")
{
    GIVEN("transform <- scaling(2, 3, 4) and v <- vector(-4, 6, 8)")
    {
        const auto transform = rtc::Matrix44::Scaling(2.0, 3.0, 4.0);
        const auto v         = rtc::Vector{ -4.0, 6.0, 8.0 };

        WHEN("The transform is applied to v")
        {
            THEN("transform * v = vector(-8, 18, 32)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, v), rtc::Vector{ -8.0, 18.0, 32.0 }));
            }
        }
    }
}

SCENARIO("Multiplying by the inverse of a scaling matrix", "[transform]")
{
    GIVEN("transform <- scaling(2, 3, 4) and inv <- inverse(transform) and v <- vector(-4, 6, 8)")
    {
        const auto transform = rtc::Matrix44::Scaling(2.0, 3.0, 4.0);
        const auto inv       = rtc::Matrix44::Inverse(transform);
        const auto v         = rtc::Vector{ -4.0, 6.0, 8.0 };

        WHEN("The inverse transform is applied to v")
        {
            THEN("inv * v = vector(-2, 2, 2)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(inv, v), rtc::Vector{ -2.0, 2.0, 2.0 }));
            }
        }
    }
}

SCENARIO("Reflection is scaling by a negative value", "[transform]")
{
    GIVEN("transform <- scaling(-1, 1, 1) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Scaling(-1.0, 1.0, 1.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(-2, 3, 4)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ -2.0, 3.0, 4.0 }));
            }
        }
    }
}

SCENARIO("Rotating a point around the x axis", "[transform]")
{
    GIVEN("p <- point(0, 1, 0) and half_quarter <- rotation_x(pi / 4) and full_quarter <- rotation_x(pi / 2)")
    {
        const auto half_quarter = rtc::Matrix44::RotationX(rtc::kPi / 4.0);
        const auto full_quarter = rtc::Matrix44::RotationX(rtc::kPi / 2.0);
        const auto p            = rtc::Point{ 0.0, 1.0, 0.0 };

        WHEN("The transforms are applied to p")
        {
            THEN("half_quarter * p = point(0, sqrt(2)/2, sqrt(2)/2) and full_quarter * p = point(0, 0, 1)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(half_quarter, p), rtc::Point{ 0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0 }));
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(full_quarter, p), rtc::Point{ 0.0, 0.0, 1.0 }));
            }
        }
    }
}

SCENARIO("Rotating a point around the y axis", "[transform]")
{
    GIVEN("p <- point(0, 0, 1) and half_quarter <- rotation_x(pi / 4) and full_quarter <- rotation_x(pi / 2)")
    {
        const auto p            = rtc::Point{ 0.0, 0.0, 1.0 };
        const auto half_quarter = rtc::Matrix44::RotationY(rtc::kPi / 4.0);
        const auto full_quarter = rtc::Matrix44::RotationY(rtc::kPi / 2.0);

        WHEN("The transforms are applied to p")
        {
            THEN("half_quarter * p = point(sqrt(2)/2, 0, sqrt(2)/2) and full_quarter * p = point(1, 0, 0)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(half_quarter, p), rtc::Point{ sqrt(2.0) / 2.0, 0.0, sqrt(2.0) / 2.0 }));
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(full_quarter, p), rtc::Point{ 1.0, 0.0, 0.0 }));
            }
        }
    }
}

SCENARIO("Rotating a point around the z axis", "[transform]")
{
    GIVEN("p <- point(0, 1, 0) and half_quarter <- rotation_x(pi / 4) and full_quarter <- rotation_x(pi / 2)")
    {
        const auto p            = rtc::Point{ 0.0, 1.0, 0.0 };
        const auto half_quarter = rtc::Matrix44::RotationZ(rtc::kPi / 4.0);
        const auto full_quarter = rtc::Matrix44::RotationZ(rtc::kPi / 2.0);

        WHEN("The transforms are applied to p")
        {
            THEN("half_quarter * p = point(-sqrt(2)/2, sqrt(2)/2, 0) and full_quarter * p = point(-1, 0, 0)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(half_quarter, p), rtc::Point{ -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0 }));
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(full_quarter, p), rtc::Point{ -1.0, 0.0, 0.0 }));
            }
        }
    }
}

SCENARIO("A shearing transformation moves x in proportion to y", "[transform]")
{
    GIVEN("transform <- shearing(1, 0, 0, 0, 0, 0) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Shearing(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(5, 3, 4)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 5.0, 3.0, 4.0 }));
            }
        }
    }
}

SCENARIO("A shearing transformation moves x in proportion to z", "[transform]")
{
    GIVEN("transform <- shearing(0, 1, 0, 0, 0, 0) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Shearing(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(6, 3, 4)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 6.0, 3.0, 4.0 }));
            }
        }
    }
}

SCENARIO("A shearing transformation moves y in proportion to x", "[transform]")
{
    GIVEN("transform <- shearing(0, 0, 1, 0, 0, 0) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Shearing(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(2, 5, 4)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 2.0, 5.0, 4.0 }));
            }
        }
    }
}

SCENARIO("A shearing transformation moves y in proportion to z", "[transform]")
{
    GIVEN("transform <- shearing(0, 0, 0, 1, 0, 0) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Shearing(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(2, 7, 4)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 2.0, 7.0, 4.0 }));
            }
        }
    }
}

SCENARIO("A shearing transformation moves z in proportion to x", "[transform]")
{
    GIVEN("transform <- shearing(0, 0, 0, 0, 1, 0) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Shearing(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(2, 3, 6)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 2.0, 3.0, 6.0 }));
            }
        }
    }
}

SCENARIO("A shearing transformation moves z in proportion to y", "[transform]")
{
    GIVEN("transform <- shearing(0, 0, 0, 0, 0, 1) and p <- point(2, 3, 4)")
    {
        const auto transform = rtc::Matrix44::Shearing(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        const auto p         = rtc::Point{ 2.0, 3.0, 4.0 };

        WHEN("The transform is applied to p")
        {
            THEN("transform * p = point(2, 3, 7)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(transform, p), rtc::Point{ 2.0, 3.0, 7.0 }));
            }
        }
    }
}

SCENARIO("Individual transformations are applied in sequence", "[transform]")
{
    GIVEN("p <- point(1, 0, 1) and A <- rotation_x(pi / 2), and B <- scaling(5, 5, 5) and C <- translation(10, 5, 7)")
    {
        const auto p = rtc::Point{ 1.0, 0.0, 1.0 };
        const auto A = rtc::Matrix44::RotationX(rtc::kPi / 2.0);
        const auto B = rtc::Matrix44::Scaling(5.0, 5.0, 5.0);
        const auto C = rtc::Matrix44::Translation(10.0, 5.0, 7.0);

        // Apply rotation first.
        WHEN("p2 <- A * p")
        {
            const auto p2 = rtc::Matrix44::Multiply(A, p);

            THEN("p2 = point(1, -1, 0)")
            {
                REQUIRE(rtc::Tuple::Equal(p2, rtc::Point{ 1.0, -1.0, 0.0 }));
            }
        }

        // Then apply scaling.
        WHEN("p3 <- B * p2")
        {
            const auto p2 = rtc::Point{ 1.0, -1.0, 0.0 };
            const auto p3 = rtc::Matrix44::Multiply(B, p2);

            THEN("p3 = point(5, -5, 0)")
            {
                REQUIRE(rtc::Tuple::Equal(p3, rtc::Point{ 5.0, -5.0, 0.0 }));
            }
        }

        // Then apply translation.
        WHEN("p4 <- C * p3")
        {
            const auto p3 = rtc::Point{ 5.0, -5.0, 0.0 };
            const auto p4 = rtc::Matrix44::Multiply(C, p3);

            THEN("p4 = point(15, 0, 7)")
            {
                REQUIRE(rtc::Tuple::Equal(p4, rtc::Point{ 15.0, 0.0, 7.0 }));
            }
        }
    }
}

SCENARIO("Chained transformations must be applied in reverse order", "[transform]")
{
    GIVEN("p <- point(1, 0, 1) and A <- rotation_x(pi / 2), and B <- scaling(5, 5, 5) and C <- translation(10, 5, 7)")
    {
        const auto p = rtc::Point{ 1.0, 0.0, 1.0 };
        const auto A = rtc::Matrix44::RotationX(rtc::kPi / 2.0);
        const auto B = rtc::Matrix44::Scaling(5.0, 5.0, 5.0);
        const auto C = rtc::Matrix44::Translation(10.0, 5.0, 7.0);

        // Apply rotation first.
        WHEN("T <- C * B * A")
        {
            const auto T = rtc::Matrix44::Multiply(C, rtc::Matrix44::Multiply(B, A));

            THEN("T * p = point(15, 0, 7)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(T, p), rtc::Point{ 15.0, 0.0, 7.0 }));
            }
        }
    }
}
