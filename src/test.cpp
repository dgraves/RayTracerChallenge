#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "double_util.h"
#include "point.h"
#include "tuple.h"
#include "vector.h"

SCENARIO("A tuple with w = 1.0 is a point", "[tuple]")
{
    GIVEN("a <- tuple(4.3, -4.2, 3.1, 1.0)")
    {
        rtc::Tuple a(4.3, -4.2, 3.1, 1.0);

        WHEN("Members are checked")
        {
            THEN("a.x = 4.3, a.y = -4.2, a.z = 3.1, a.w = 1.0, a is a point, a is not a vector")
            {
                REQUIRE(rtc::Equal(a.GetX(), 4.3));
                REQUIRE(rtc::Equal(a.GetY(), -4.2));
                REQUIRE(rtc::Equal(a.GetZ(), 3.1));
                REQUIRE(rtc::Equal(a.GetW(), 1.0));
                REQUIRE(a.IsPoint());
                REQUIRE(!a.IsVector());
            }
        }
    }
}

SCENARIO("A tuple with w = 0.0 is a vector", "[tuple]")
{
    GIVEN("a <- tuple(4.3, -4.2, 3.1, 0.0)")
    {
        rtc::Tuple a(4.3, -4.2, 3.1, 0.0);

        WHEN("Members are checked")
        {
            THEN("a.x = 4.3, a.y = -4.2, a.z = 3.1, a.w = 0.0, a is not a point, a is a vector")
            {
                REQUIRE(rtc::Equal(a.GetX(), 4.3));
                REQUIRE(rtc::Equal(a.GetY(), -4.2));
                REQUIRE(rtc::Equal(a.GetZ(), 3.1));
                REQUIRE(rtc::Equal(a.GetW(), 0.0));
                REQUIRE(!a.IsPoint());
                REQUIRE(a.IsVector());
            }
        }
    }
}

SCENARIO("Point creates tuples with w = 1.0", "[tuple]")
{
    GIVEN("p <- point(4.0, -4.0, 3.0)")
    {
        rtc::Point p(4.0, -4.0, 3.0);
        rtc::Tuple a(4.0, -4.0, 3.0, 1.0);

        WHEN("Members are checked")
        {
            THEN("p == tuple(4.0, -4.0, 3.0, 1.0)")
            {
                REQUIRE(rtc::Tuple::Equal(p, a));
            }
        }
    }
}

SCENARIO("Vector creates tuples with w = 0.0", "[tuple]")
{
    GIVEN("v <- point(4.0, -4.0, 3.0)")
    {
        rtc::Vector v(4.0, -4.0, 3.0);
        rtc::Tuple a(4.0, -4.0, 3.0, 0.0);

        WHEN("Members are checked")
        {
            THEN("v == tuple(4.0, -4.0, 3.0, 0.0)")
            {
                REQUIRE(rtc::Tuple::Equal(v, a));
            }
        }
    }
}

SCENARIO("Adding two tuples", "[tuple]")
{
    GIVEN("a1 <- tuple(3, -2, 5, 1) and a2 <- tuple(-2, 3, 1, 0)")
    {
        rtc::Tuple a1(3, -2, 5, 1);
        rtc::Tuple a2(-2, 3, 1, 0);

        WHEN("Tuples are added")
        {
            THEN("a1 + a2 == tuple(1, 1, 6, 1)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Add(a1, a2), rtc::Tuple(1, 1, 6, 1)));

                a1.Add(a2);
                REQUIRE(a1.Equal(rtc::Tuple(1, 1, 6, 1)));
            }
        }
    }
}

SCENARIO("Subtracting two points", "[tuple]")
{
    GIVEN("p1 <- point(3, 2, 1) and p2 <- point(5, 6, 7)")
    {
        rtc::Point p1(3, 2, 1);
        rtc::Point p2(5, 6, 7);

        WHEN("Points are subtracted")
        {
            THEN("p1 - p2 == vector(-2, -4, -6)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Subtract(p1, p2), rtc::Vector(-2, -4, -6)));

                p1.Subtract(p2);
                REQUIRE(p1.Equal(rtc::Vector(-2, -4, -6)));
            }
        }
    }
}

SCENARIO("Subtracting a vector from a point", "[tuple]")
{
    GIVEN("p <- point(3, 2, 1) and v <- vector(5, 6, 7)")
    {
        rtc::Point p(3, 2, 1);
        rtc::Vector v(5, 6, 7);

        WHEN("Vector is subtracted from point")
        {
            THEN("p - v == point(-2, -4, -6)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Subtract(p, v), rtc::Point(-2, -4, -6)));

                p.Subtract(v);
                REQUIRE(p.Equal(rtc::Point(-2, -4, -6)));
            }
        }
    }
}

SCENARIO("Subtracting two vectors", "[tuple]")
{
    GIVEN("v1 <- vector(3, 2, 1) and v2 <- vector(5, 6, 7)")
    {
        rtc::Vector v1(3, 2, 1);
        rtc::Vector v2(5, 6, 7);

        WHEN("Vectors are subtracted")
        {
            THEN("v1 - v2 == vector(-2, -4, -6)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Subtract(v1, v2), rtc::Vector(-2, -4, -6)));

                v1.Subtract(v2);
                REQUIRE(v1.Equal(rtc::Vector(-2, -4, -6)));
            }
        }
    }
}

SCENARIO("Subtracting a vector from the zero vector", "[tuple]")
{
    GIVEN("zero <- vector(3, 2, 1) and v2 <- vector(5, 6, 7)")
    {
        rtc::Vector zero(0, 0, 0);
        rtc::Vector v(1, -2, 3);

        WHEN("Vector is subtracted from zero vector")
        {
            THEN("zero - v == vector(-1, 2, -3)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Subtract(zero, v), rtc::Vector(-1, 2, -3)));
            }
        }
    }
}

SCENARIO("Negating a tuple", "[tuple]")
{
    GIVEN("a <- vector(1, -2, 3, -4)")
    {
        rtc::Tuple a(1, -2, 3, -4);

        WHEN("Tuple is negated")
        {
            THEN("-a == tuple(-1, 2, -3, 4)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Negate(a), rtc::Tuple(-1, 2, -3, 4)));

                a.Negate();
                REQUIRE(a.Equal(rtc::Tuple(-1, 2, -3, 4)));
            }
        }
    }
}

SCENARIO("Multiplying a tuple by a scalar", "[tuple]")
{
    GIVEN("a <- vector(1, -2, 3, -4)")
    {
        rtc::Tuple a(1, -2, 3, -4);

        WHEN("Tuple is multiplied by 3.5")
        {
            THEN("a * 3.5 == tuple(3.5, -7, 10.5, -14)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Multiply(a, 3.5), rtc::Tuple(3.5, -7, 10.5, -14)));

                a.Multiply(3.5);
                REQUIRE(a.Equal(rtc::Tuple(3.5, -7, 10.5, -14)));
            }
        }
    }
}

SCENARIO("Multiplying a tuple by a fraction", "[tuple]")
{
    GIVEN("a <- vector(1, -2, 3, -4)")
    {
        rtc::Tuple a(1, -2, 3, -4);

        WHEN("Tuple is multiplied by 0.5")
        {
            THEN("a * 0.5 == tuple(0.5, -1, 1.5, -2)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Multiply(a, 0.5), rtc::Tuple(0.5, -1, 1.5, -2)));

                a.Multiply(0.5);
                REQUIRE(a.Equal(rtc::Tuple(0.5, -1, 1.5, -2)));
            }
        }
    }
}

SCENARIO("Dividing a tuple by a scalar", "[tuple]")
{
    GIVEN("a <- vector(1, -2, 3, -4)")
    {
        rtc::Tuple a(1, -2, 3, -4);

        WHEN("Tuple is divided by 2")
        {
            THEN("a / 2 == tuple(0.5, -1, 1.5, -2)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Tuple::Divide(a, 2), rtc::Tuple(0.5, -1, 1.5, -2)));

                a.Divide(2);
                REQUIRE(a.Equal(rtc::Tuple(0.5, -1, 1.5, -2)));
            }
        }
    }
}

SCENARIO("Computing the magnitude of vectors", "[vector]")
{
    GIVEN("A series of vectors")
    {
        WHEN("v <- vector(1, 0, 0)")
        {
            THEN("magnitude(v) == 1")
            {
                rtc::Vector v(1.0, 0.0, 0.0);
                REQUIRE(rtc::Equal(v.Magnitude(), 1.0));
            }
        }

        WHEN("v <- vector(0, 1, 0)")
        {
            THEN("magnitude(v) == 1")
            {
                rtc::Vector v(0.0, 1.0, 0.0);
                REQUIRE(rtc::Equal(v.Magnitude(), 1.0));
            }
        }

        WHEN("v <- vector(0, 0, 1)")
        {
            THEN("magnitude(v) == 1")
            {
                rtc::Vector v(0.0, 0.0, 1.0);
                REQUIRE(rtc::Equal(v.Magnitude(), 1.0));
            }
        }

        WHEN("v <- vector(1, 2, 3)")
        {
            THEN("magnitude(v) == sqrt(14)")
            {
                rtc::Vector v(1.0, 2.0, 3.0);
                REQUIRE(rtc::Equal(v.Magnitude(), sqrt(14.0)));
            }
        }

        WHEN("v <- vector(-1, -2, -3)")
        {
            THEN("magnitude(v) == sqrt(14)")
            {
                rtc::Vector v(-1.0, -2.0, -3.0);
                REQUIRE(rtc::Equal(v.Magnitude(), sqrt(14.0)));
            }
        }
    }
}

SCENARIO("Normalizing vectors", "[vector]")
{
    GIVEN("A series of vectors")
    {
        WHEN("v <- vector(4, 0, 0)")
        {
            THEN("normalize(v) == vector(1, 0, 0)")
            {
                rtc::Vector v(4.0, 0.0, 0.0);
                REQUIRE(rtc::Tuple::Equal(rtc::Vector::Normalize(v), rtc::Vector(1.0, 0.0, 0.0)));

                v.Normalize();
                REQUIRE(v.Equal(rtc::Vector(1.0, 0.0, 0.0)));
            }
        }

        WHEN("v <- vector(1, 2, 3)")
        {
            THEN("normalize(v) == vector(1 / sqrt(14), 2 / sqrt(14), 3 / sqrt(14))")
            {
                rtc::Vector v(1.0, 2.0, 3.0);
                REQUIRE(rtc::Tuple::Equal(rtc::Vector::Normalize(v), rtc::Vector(0.26726, 0.53452, 0.80178)));

                v.Normalize();
                REQUIRE(v.Equal(rtc::Vector(0.26726, 0.53452, 0.80178)));
            }
        }

        WHEN("v <- vector(1, 2, 3)")
        {
            THEN("magnitude(normalize(v)) == 1")
            {
                rtc::Vector v(1.0, 2.0, 3.0);
                REQUIRE(rtc::Equal(rtc::Vector::Normalize(v).Magnitude(), 1.0));

                v.Normalize();
                REQUIRE(rtc::Equal(v.Magnitude(), 1.0));
            }
        }
    }
}

SCENARIO("The dot product of two vectors", "[vector]")
{
    GIVEN("a <- vector(1, 2, 3) and b <- vector(2, 3, 4)")
    {
        WHEN("The dot product is computed between a and b")
        {
            THEN("dot(a, b) = 20")
            {
                rtc::Vector a(1.0, 2.0, 3.0);
                rtc::Vector b(2.0, 3.0, 4.0);
                REQUIRE(rtc::Equal(rtc::Vector::Dot(a, b), 20.0));
                REQUIRE(rtc::Equal(a.Dot(b), 20.0));
            }
        }
    }
}

SCENARIO("The cross product of two vectors")
{
    GIVEN("a <-vector(1, 2, 3) and b <- vector(2, 3, 4)")
    {
        WHEN("The cross product is computed between a and b")
        {
            THEN("cross(a, b) = vector(-1, 2, -1) and cross(b, a) = vector(1, -2, 1)")
            {
                rtc::Vector a(1.0, 2.0, 3.0);
                rtc::Vector b(2.0, 3.0, 4.0);
                REQUIRE(rtc::Tuple::Equal(rtc::Vector::Cross(a, b), rtc::Vector(-1.0, 2.0, -1.0)));
                REQUIRE(rtc::Tuple::Equal(rtc::Vector::Cross(b, a), rtc::Vector(1.0, -2.0, 1.0)));
            }
        }
    }
}
