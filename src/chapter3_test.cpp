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
#include "matrix.h"
#include "matrix22.h"
#include "matrix33.h"
#include "matrix44.h"
#include "tuple.h"

SCENARIO("Constructing and inspecting a 4x4 matrix", "[matrix]")
{
    GIVEN("The following 4x4 matrix M")
    {
        WHEN("M is { { 1, 2, 3, 4 }, { 5.5, 6.5, 7.5, 8.5 }, { 9, 10, 11, 12 }, { 13.5, 14.5, 15.5, 16.5 } }")
        {
            const auto M = rtc::Matrix<4, 4>{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 5.5, 6.5, 7.5, 8.5 }},
                {{ 9.0, 10.0, 11.0, 12.0 }},
                {{ 13.5, 14.5, 15.5, 16.5 }}
                }} };

            THEN("Then M[0,0] = 1 and M[0, 3] = 4 and M[1, 0] = 5.5 and M[1, 2] = 7.5 and M[2, 2] = 11 and M[3, 0] = 13.5 and M[3, 2] = 15.5")
            {
                REQUIRE(rtc::Equal(M.Get(0u, 0u), 1.0));
                REQUIRE(rtc::Equal(M.Get(0u, 3u), 4.0));
                REQUIRE(rtc::Equal(M.Get(1u, 0u), 5.5));
                REQUIRE(rtc::Equal(M.Get(1u, 2u), 7.5));
                REQUIRE(rtc::Equal(M.Get(2u, 2u), 11.0));
                REQUIRE(rtc::Equal(M.Get(3u, 0u), 13.5));
                REQUIRE(rtc::Equal(M.Get(3u, 2u), 15.5));
            }
        }
    }
}

SCENARIO(" A 2x2 matrix ought to be representable", "[matrix]")
{
    GIVEN("The following 2x2 matrix M")
    {
        auto M = rtc::Matrix<2, 2>{};

        WHEN("M is { { -3, 5 }, { 1, -2 } }")
        {
            M.Set(0u, 0u, -3.0);
            M.Set(0u, 1u, 5.0);
            M.Set(1u, 0u, 1.0);
            M.Set(1u, 1u, -2.0);

            THEN("Then M[0,0] = -3 and M[0, 1] = 5 and M[1, 0] = 1 and M[1, 1] = -2")
            {
                REQUIRE(rtc::Equal(M.Get(0u, 0u), -3.0));
                REQUIRE(rtc::Equal(M.Get(0u, 1u), 5.0));
                REQUIRE(rtc::Equal(M.Get(1u, 0u), 1.0));
                REQUIRE(rtc::Equal(M.Get(1u, 1u), -2.0));
            }
        }
    }
}

SCENARIO(" A 3x3 matrix ought to be representable", "[matrix]")
{
    GIVEN("The following 3x3 matrix M")
    {
        WHEN("M is { { -3, 5, 0 }, { 1, -2, -7 }, { 0, 1, 1 } }")
        {
            const double data[3][3] = { { -3.0, 5.0, 0.0 }, { 1.0, -2.0, -7.0 }, { 0.0, 1.0, 1.0 } };
            const auto   M          = rtc::Matrix<3, 3>{ data };

            THEN("Then M[0,0] = -3 and M[1, 1] = -2 and M[2, 2] = 1")
            {
                REQUIRE(rtc::Equal(M.Get(0u, 0u), -3.0));
                REQUIRE(rtc::Equal(M.Get(1u, 1u), -2.0));
                REQUIRE(rtc::Equal(M.Get(2u, 2u), 1.0));
            }
        }
    }
}

SCENARIO("Matrix equality with identical matrices", "[matrix]")
{
    GIVEN("The following matrices A and B")
    {
        WHEN("A and B are { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } }")
        {
            const auto A = rtc::Matrix<4, 4>{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 5.0, 6.0, 7.0, 8.0 }},
                {{ 9.0, 10.0, 11.0, 12.0 }},
                {{ 13.0, 14.0, 15.0, 16.0 }}
                }} };

            const auto B = rtc::Matrix<4, 4>{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 5.0, 6.0, 7.0, 8.0 }},
                {{ 9.0, 10.0, 11.0, 12.0 }},
                {{ 13.0, 14.0, 15.0, 16.0 }}
                }} };

            THEN("A = B")
            {
                REQUIRE(A.Equal(B));
            }
        }
    }
}

SCENARIO("Matrix equality with different matrices", "[matrix]")
{
    GIVEN("The following matrices A and B")
    {
        WHEN("A is { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } } and B is { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 8, 7, 6 }, { 5, 4, 3, 2 } }")
        {
            const auto A = rtc::Matrix<4, 4>{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 5.0, 6.0, 7.0, 8.0 }},
                {{ 9.0, 10.0, 11.0, 12.0 }},
                {{ 13.0, 14.0, 15.0, 16.0 }}
                }} };

            const auto B = rtc::Matrix<4, 4>{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 5.0, 6.0, 7.0, 8.0 }},
                {{ 9.0, 8.0, 7.0, 6.0 }},
                {{ 5.0, 4.0, 3.0, 2.0 }}
                }} };

            THEN("A = B")
            {
                REQUIRE(!rtc::Matrix<4, 4>::Equal(A, B));
            }
        }
    }
}

SCENARIO("Multiplying two matrices", "[matrix]")
{
    GIVEN("The following matrices A and B")
    {
        WHEN("A is { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 8, 7, 6 }, { 5, 4, 3, 2 } } and B is { { -2, 1, 2, 3 }, { 3, 2, 1, -1 }, { 4, 3, 6, 5 }, { 1, 2, 7, 8 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 5.0, 6.0, 7.0, 8.0 }},
                {{ 9.0, 8.0, 7.0, 6.0 }},
                {{ 5.0, 4.0, 3.0, 2.0 }}
                }} };

            const auto B = rtc::Matrix44{ {{
                {{ -2.0, 1.0, 2.0, 3.0 }},
                {{ 3.0, 2.0, 1.0, -1.0 }},
                {{ 4.0, 3.0, 6.0, 5.0 }},
                {{ 1.0, 2.0, 7.0, 8.0 }}
                }} };

            THEN("A * B is { { 20, 22, 50, 48 }, { 44, 54, 114, 108 }, { 40, 58, 110, 102 }, { 16, 26, 46, 42 } }")
            {
                const auto expected = rtc::Matrix44{ {{
                    {{ 20.0, 22.0, 50.0, 48.0 }},
                    {{ 44.0, 54.0, 114.0, 108.0 }},
                    {{ 40.0, 58.0, 110.0, 102.0 }},
                    {{ 16.0, 26.0, 46.0, 42.0 }}
                    }} };

                REQUIRE(rtc::Matrix44::Equal(rtc::Matrix44::Multiply(A, B), expected));
            }
        }
    }
}

SCENARIO("A matrix multiplied by a tuple", "[matrix]")
{
    GIVEN("The following matrix A and tuple b")
    {
        WHEN("A is { { 1, 2, 3, 4 }, { 2, 4, 4, 2 }, { 8, 6, 4, 1 }, { 0, 0, 0, 1 } } and b <- tuple(1, 2, 3, 1)")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 1.0, 2.0, 3.0, 4.0 }},
                {{ 2.0, 4.0, 4.0, 2.0 }},
                {{ 8.0, 6.0, 4.0, 1.0 }},
                {{ 0.0, 0.0, 0.0, 1.0 }}
                }} };

            const auto b = rtc::Tuple{ 1.0, 2.0, 3.0, 1.0 };

            THEN("A * b = tuple(18, 24, 33, 1)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(A, b), rtc::Tuple{ 18.0, 24.0, 33.0, 1.0 }));
            }
        }
    }
}

SCENARIO("Multiplying a matrix by the identity matrix", "[matrix]")
{
    GIVEN("The following matrix A")
    {
        WHEN("A is { { 0, 1, 2, 4 }, { 1, 2, 4, 8 }, { 2, 4, 8, 16 }, { 4, 8, 16, 32 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 0, 1, 2, 4 }},
                {{ 1, 2, 4, 8 }},
                {{ 2, 4, 8, 16 }},
                {{ 4, 8, 16, 32 }}
                }} };

            THEN("A * identity_matrix = A")
            {
                REQUIRE(rtc::Matrix44::Equal(rtc::Matrix44::Multiply(A, rtc::Matrix44::Identity()), A));
            }
        }
    }
}

SCENARIO("Multiplying the identity matrix by a tuple", "[matrix]")
{
    GIVEN("a <- tuple(1, 2, 3, 4)")
    {
        const auto a = rtc::Tuple{ 1.0, 2.0, 3.0, 4.0 };

        WHEN("Multiplying the identity matrix by a")
        {
            THEN("identity_matrix * a = a")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(rtc::Matrix44::Identity(), a), a));
            }
        }
    }
}

SCENARIO("Transposing a matrix", "[matrix]")
{
    GIVEN("The following matrix A")
    {
        WHEN("A is { { 0, 9, 3, 0 }, { 9, 8, 0, 8 }, { 1, 8, 5, 3 }, { 0, 0, 5, 8 } }")
        {
            auto A = rtc::Matrix44{ {{
                {{ 0.0, 9.0, 3.0, 0.0 }},
                {{ 9.0, 8.0, 0.0, 8.0 }},
                {{ 1.0, 8.0, 5.0, 3.0 }},
                {{ 0.0, 0.0, 5.0, 8.0 }}
                }} };

            THEN("Transpose(A) is { { 0, 9, 1, 0 }, { 9, 8, 8, 0 }, { 3, 0, 5, 5 }, { 0, 8, 3, 8 } }")
            {
                A.Transpose();

                const auto expected = rtc::Matrix44{ {{
                    {{ 0.0, 9.0, 1.0, 0.0 }},
                    {{ 9.0, 8.0, 8.0, 0.0 }},
                    {{ 3.0, 0.0, 5.0, 5.0 }},
                    {{ 0.0, 8.0, 3.0, 8.0 }}
                    }} };

                REQUIRE(rtc::Matrix44::Equal(A, expected));
            }
        }
    }
}

SCENARIO("Transposing the identity matrix", "[matrix]")
{
    GIVEN("The following matrix A")
    {
        WHEN("A <- transpose(identity_matrix)")
        {
            const auto identity = rtc::Matrix44::Identity();
            const auto A        = rtc::Matrix44::Transpose(identity);

            THEN("A = identity_matrix")
            {
                REQUIRE(rtc::Matrix44::Equal(A, identity));
            }
        }
    }
}

SCENARIO("Calculating the determinant of a 2x2 matrix", "[matrix]")
{
    GIVEN("The following matrix A")
    {
        WHEN("A is { { 1, 5 }, { -3, 2 } }")
        {
            const auto A = rtc::Matrix22{ {{ {{ 1.0, 5.0 }}, {{ -3.0, 2.0 }} }} };

            THEN("determinant(A) = 17")
            {
                REQUIRE(rtc::Equal(A.Determinant(), 17.0));
            }
        }
    }
}

SCENARIO("A submatrix of a 3x3 matrix is a 2x2 matrix", "[matrix]")
{
    GIVEN("The following 3x3 matrix A")
    {
        WHEN("A is { { 1, 5, 0 }, { -3, 2, 7 }, { 0, 6, -3 } }")
        {
            const auto A = rtc::Matrix33{ {{ {{ 1.0, 5.0, 0.0 }}, {{ -3.0, 2.0, 7.0 }}, {{ 0.0, 6.0, -3.0 }} }} };

            THEN("submatrix(A, 0, 2) is the following 2x2 matrix { { -3, 2 }, { 0, 6 } }")
            {
                const auto expected = rtc::Matrix22{ {{ {{ -3.0, 2.0 }}, {{ 0.0, 6.0 }} }} };

                REQUIRE(rtc::Matrix22::Equal(A.Submatrix(0u, 2u), expected));
            }
        }
    }
}

SCENARIO("A submatrix of a 4x4 matrix is a 3x3 matrix", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { -6, 1, 1, 6 }, { -8, 5, 8, 6 }, { -1, 0, 8, 2 }, { -7, 1, -1, 1 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ -6.0, 1.0, 1.0, 6.0 }},
                {{ -8.0, 5.0, 8.0, 6.0 }},
                {{ -1.0, 0.0, 8.0, 2.0 }},
                {{ -7.0, 1.0, -1.0, 1.0 }}
                }} };

            THEN("submatrix(A, 2, 1) is the following 3x3 matrix { { -6, 1, 6 }, { -8, 8, 6 }, { -7, -1, 1 } }")
            {
                const auto expected = rtc::Matrix33{ {{ {{ -6.0, 1.0, 6.0 }}, {{ -8.0, 8.0, 6.0 }}, {{ -7.0, -1.0, 1.0 }} }} };

                REQUIRE(rtc::Matrix33::Equal(A.Submatrix(2u, 1u), expected));
            }
        }
    }
}

SCENARIO("Calculating a minor of a 3x3 matrix", "[matrix]")
{
    GIVEN("The following 3x3 matrix A")
    {
        WHEN("A is { { 3, 5, 0 }, { 2, -1, -7 }, { 6, -1, 5 } } and B <- submatrix(A, 1, 0)")
        {
            const auto A = rtc::Matrix33{ {{
                {{ 3.0, 5.0, 0.0 }},
                {{ 2.0, -1.0, -7.0 }},
                {{ 6.0, -1.0, 5.0 }}
                }} };

            THEN("determinant(B) = 25 and minor(A, 1, 0) = 25")
            {
                const auto B = rtc::Matrix33::Submatrix(A, 1u, 0u);

                REQUIRE(rtc::Equal(B.Determinant(), 25.0));
                REQUIRE(rtc::Equal(A.Minor(1u, 0u), 25.0));
            }
        }
    }
}

SCENARIO("Calculating a cofactor of a 3x3 matrix", "[matrix]")
{
    GIVEN("The following 3x3 matrix A")
    {
        WHEN("A is { { 3, 5, 0 }, { 2, -1, -7 }, { 6, -1, 5 } }")
        {
            const auto A = rtc::Matrix33{ {{
                {{ 3.0, 5.0, 0.0 }},
                {{ 2.0, -1.0, -7.0 }},
                {{ 6.0, -1.0, 5.0 }}
                }} };

            THEN("minor(A, 0, 0) = -12 and cofactor(A, 0, 0) = -12 and minor(A, 1, 0) = 25 and cofactor(A, 1, 0) = -25")
            {
                REQUIRE(rtc::Equal(A.Minor(0u, 0u), -12.0));
                REQUIRE(rtc::Equal(A.Cofactor(0u, 0u), -12.0));
                REQUIRE(rtc::Equal(rtc::Matrix33::Minor(A, 1u, 0u), 25.0));
                REQUIRE(rtc::Equal(rtc::Matrix33::Cofactor(A, 1u, 0u), -25.0));
            }
        }
    }
}

SCENARIO("Calculating the determinant of a 3x3 matrix", "[matrix]")
{
    GIVEN("The following 3x3 matrix A")
    {
        WHEN("A is { { 1, 2, 6 }, { -5, 8, -4 }, { 2, 6, 4 } }")
        {
            const auto A = rtc::Matrix33{ {{
                {{ 1.0, 2.0, 6.0 }},
                {{ -5.0, 8.0, -4.0 }},
                {{ 2.0, 6.0, 4.0 }}
                }} };

            THEN("cofactor(A, 0, 0) = 56 and cofactor(A, 0, 1) = 12 and cofactor(A, 0, 2) = -46 and determinant(A) = -196")
            {
                REQUIRE(rtc::Equal(A.Cofactor(0u, 0u), 56.0));
                REQUIRE(rtc::Equal(A.Cofactor(0u, 1u), 12.0));
                REQUIRE(rtc::Equal(rtc::Matrix33::Cofactor(A, 0u, 2u), -46.0));
                REQUIRE(rtc::Equal(A.Determinant(), -196.0));
            }
        }
    }
}

SCENARIO("Calculating the determinant of a 4x4 matrix", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { -2, -8, 3, 5 }, { -3, 1, 7, 3 }, { 1, 2, -9, 6 }, { -6, 7, 7, -9 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ -2.0, -8.0, 3.0, 5.0 }},
                {{ -3.0, 1.0, 7.0, 3.0 }},
                {{ 1.0, 2.0, -9.0, 6.0 }},
                {{ -6.0, 7.0, 7.0, -9.0 }}
                }} };

            THEN("cofactor(A, 0, 0) = 690 and cofactor(A, 0, 1) = 447 and cofactor(A, 0, 2) = 210 and cofactor(A, 0, 3) = 51 and determinant(A) = -4071")
            {
                REQUIRE(rtc::Equal(A.Cofactor(0u, 0u), 690.0));
                REQUIRE(rtc::Equal(A.Cofactor(0u, 1u), 447.0));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 0u, 2u), 210.0));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 0u, 3u), 51.0));
                REQUIRE(rtc::Equal(A.Determinant(), -4071.0));
            }
        }
    }
}

SCENARIO("Testing an invertible matrix for invertibility", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { 6, 4, 4, 4 }, { 5, 5, 7, 6 }, { 4, -9, 3, -7 }, { 9, 1, 7, -6 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 6.0, 4.0, 4.0, 4.0 }},
                {{ 5.0, 5.0, 7.0, 6.0 }},
                {{ 4.0, -9.0, 3.0, -7.0 }},
                {{ 9.0, 1.0, 7.0, -6.0 }}
                }} };

            THEN("determinant(A) = -2120 and A is invertible")
            {
                REQUIRE(rtc::Equal(rtc::Matrix44::Determinant(A), -2120.0));
                REQUIRE(rtc::Matrix44::IsInvertible(A));
            }
        }
    }
}

SCENARIO("Testing a non-invertible matrix for invertibility", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { -4, 2, -2, -3 }, { 9, 6, 2, 6 }, { 0, -5, 1, -5 }, { 0, 0, 0, 0 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ -4.0, 2.0, -2.0, -3.0 }},
                {{ 9.0, 6.0, 2.0, 6.0 }},
                {{ 0.0, -5.0, 1.0, -5.0 }},
                {{ 0.0, 0.0, 0.0, 0.0 }}
                }} };

            THEN("determinant(A) = 0 and A is not invertible")
            {
                REQUIRE(rtc::Equal(rtc::Matrix44::Determinant(A), 0.0));
                REQUIRE(!rtc::Matrix44::IsInvertible(A));
            }
        }
    }
}

SCENARIO("Calculating the inverse of a matrix", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { -5, 2, 6, -8 }, { 1, -5, 1, 8 }, { 7, 7, -6, -7 }, { 1, -3, 7, 4 } } and B <- inverse(A)")
        {
            const auto A = rtc::Matrix44{ {{
                {{ -5.0, 2.0, 6.0, -8.0 }},
                {{ 1.0, -5.0, 1.0, 8.0 }},
                {{ 7.0, 7.0, -6.0, -7.0 }},
                {{ 1.0, -3.0, 7.0, 4.0 }}
                }} };
            const auto B = rtc::Matrix44::Inverse(A);

            THEN("determinant(A) = 532 and cofactor(A, 2, 3) = -160 and B[3, 2] = -160/532 and cofactor(A, 3, 2) = 105 and B[2, 3] = 105/532 and B is { { 0.21805, 0.45113, 0.24060, -0.04511 }, { -0.80827, -1.45677, -0.44361, 0.52068 }, { -0.07895, -0.22368, -0.05263, 0.19737 }, { -0.52256, -0.81391, -0.30075, 0.30639 } }")
            {
                const auto expected = rtc::Matrix44{ { {
                    {{ 0.21805, 0.45113, 0.24060, -0.04511 }},
                    {{ -0.80827, -1.45677, -0.44361, 0.52068 }},
                    {{ -0.07895, -0.22368, -0.05263, 0.19737 }},
                    {{  -0.52256, -0.81391, -0.30075, 0.30639 }}
                    }} };

                REQUIRE(rtc::Equal(rtc::Matrix44::Determinant(A), 532.0));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 2u, 3u), -160.0));
                REQUIRE(rtc::Equal(B.Get(3u, 2u), -160.0 / 532.0));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 3u, 2u), 105.0));
                REQUIRE(rtc::Equal(B.Get(2u, 3u), 105.0 / 532.0));
                REQUIRE(rtc::Matrix44::Equal(B, expected));
            }
        }
    }
}

SCENARIO("Calculating the inverse of another matrix", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { 8, -5, 9, 2 }, { 7, 5, 6, 1 }, { -6, 0, 9, 6 }, { -3, 0, -9, -4 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 8.0, -5.0, 9.0, 2.0 }},
                {{ 7.0, 5.0, 6.0, 1.0 }},
                {{ -6.0, 0.0, 9.0, 6.0 }},
                {{ -3.0, 0.0, -9.0, -4.0 }}
                }} };

            THEN("inverse(A) is { { -0.15385, -0.15385, -0.28205, -0.53846 }, { -0.07692, 0.12308, 0.02564, 0.03077 }, { 0.35897, 0.35897, 0.43590, 0.92308 }, { -0.69231, -0.69231, -0.76923, -1.92308 } }")
            {
                const auto expected = rtc::Matrix44{ {{
                    {{ -0.15385, -0.15385, -0.28205, -0.53846 }},
                    {{ -0.07692, 0.12308, 0.02564, 0.03077 }},
                    {{ 0.35897, 0.35897, 0.43590, 0.92308 }},
                    {{ -0.69231, -0.69231, -0.76923, -1.92308 }}
                    }} };

                REQUIRE(rtc::Matrix44::Equal(rtc::Matrix44::Inverse(A), expected));
            }
        }
    }
}

SCENARIO("Calculating the inverse of a third matrix", "[matrix]")
{
    GIVEN("The following 4x4 matrix A")
    {
        WHEN("A is { { 9, 3, 0, 9 }, { -5, -2, -6, -3 }, { -4, 9, 6, 4 }, { -7, 6, 6, 2 } }")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 9.0, 3.0, 0.0, 9.0 }},
                {{ -5.0, -2.0, -6.0, -3.0 }},
                {{ -4.0, 9.0, 6.0, 4.0 }},
                {{ -7.0, 6.0, 6.0, 2.0 }}
                }} };

            THEN("inverse(A) is { { -0.04074, -0.07778, 0.14444, -0.22222 }, { -0.07778, 0.03333, 0.36667, -0.33333 }, { -0.02901, -0.14630, -0.10926, 0.12963 }, { 0.17778, 0.06667, -0.26667, 0.33333 } }")
            {
                const auto expected = rtc::Matrix44{ {{
                    {{ -0.04074, -0.07778, 0.14444, -0.22222 }},
                    {{ -0.07778, 0.03333, 0.36667, -0.33333 }},
                    {{ -0.02901, -0.14630, -0.10926, 0.12963 }},
                    {{ 0.17778, 0.06667, -0.26667, 0.33333 }}
                    }} };

                REQUIRE(rtc::Matrix44::Equal(rtc::Matrix44::Inverse(A), expected));
            }
        }
    }
}

SCENARIO("Multiplying a product by its inverse", "[matrix]")
{
    GIVEN("The following 4x4 matrices A and B")
    {
        WHEN("A is { { 3, -9, 7, 3 }, { 3, -8, 2, -9 }, { -4, 4, 4, 1 }, { -6, 5, -1, 1 } } and B is { { 8, 2, 2, 2 }, { 3, -1, 7, 0 }, { 7, 0, 5, 4 }, { 6, -2, 0, 5 } } and C <- A * B")
        {
            const auto A = rtc::Matrix44{ {{
                {{ 3.0, -9.0, 7.0, 3.0 }},
                {{ 3.0, -8.0, 2.0, -9.0 }},
                {{ -4.0, 4.0, 4.0, 1.0 }},
                {{ -6.0, 5.0, -1.0, 1.0 }}
                }} };
            const auto B = rtc::Matrix44{ {{
                {{ 8.0, 2.0, 2.0, 2.0 }},
                {{ 3.0, -1.0, 7.0, 0.0 }},
                {{ 7.0, 0.0, 5.0, 4.0 }},
                {{ 6.0, -2.0, 0.0, 5.0 }}
                }} };
            const auto C = rtc::Matrix44::Multiply(A, B);

            THEN("C * inverse(B) = A")
            {
                REQUIRE(rtc::Matrix44::Equal(rtc::Matrix44::Multiply(C, rtc::Matrix44::Inverse(B)), A));
            }
        }
    }
}
