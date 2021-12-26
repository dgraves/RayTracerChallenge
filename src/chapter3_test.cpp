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
            rtc::Matrix<4, 4> M({{
                {{ 1, 2, 3, 4 }},
                {{ 5.5, 6.5, 7.5, 8.5 }},
                {{ 9, 10, 11, 12 }},
                {{ 13.5, 14.5, 15.5, 16.5 }}
                }});

            THEN("Then M[0,0] = 1 and M[0, 3] = 4 and M[1, 0] = 5.5 and M[1, 2] = 7.5 and M[2, 2] = 11 and M[3, 0] = 13.5 and M[3, 2] = 15.5")
            {
                REQUIRE(rtc::Equal(M.Get(0, 0), 1));
                REQUIRE(rtc::Equal(M.Get(0, 3), 4));
                REQUIRE(rtc::Equal(M.Get(1, 0), 5.5));
                REQUIRE(rtc::Equal(M.Get(1, 2), 7.5));
                REQUIRE(rtc::Equal(M.Get(2, 2), 11));
                REQUIRE(rtc::Equal(M.Get(3, 0), 13.5));
                REQUIRE(rtc::Equal(M.Get(3, 2), 15.5));
            }
        }
    }
}

SCENARIO(" A 2x2 matrix ought to be representable", "[matrix]")
{
    GIVEN("The following 2x2 matrix M")
    {
        rtc::Matrix<2, 2> M;

        WHEN("M is { { -3, 5 }, { 1, -2 } }")
        {
            M.Set(0, 0, -3);
            M.Set(0, 1, 5);
            M.Set(1, 0, 1);
            M.Set(1, 1, -2);

            THEN("Then M[0,0] = -3 and M[0, 1] = 5 and M[1, 0] = 1 and M[1, 1] = -2")
            {
                REQUIRE(rtc::Equal(M.Get(0, 0), -3));
                REQUIRE(rtc::Equal(M.Get(0, 1), 5));
                REQUIRE(rtc::Equal(M.Get(1, 0), 1));
                REQUIRE(rtc::Equal(M.Get(1, 1), -2));
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
            double            data[3][3] = { { -3, 5, 0 }, { 1, -2, -7 }, { 0, 1, 1 } };
            rtc::Matrix<3, 3> M(data);

            THEN("Then M[0,0] = -3 and M[1, 1] = -2 and M[2, 2] = 1")
            {
                REQUIRE(rtc::Equal(M.Get(0, 0), -3));
                REQUIRE(rtc::Equal(M.Get(1, 1), -2));
                REQUIRE(rtc::Equal(M.Get(2, 2), 1));
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
            rtc::Matrix<4, 4> A({{
                {{ 1, 2, 3, 4 }},
                {{ 5, 6, 7, 8 }},
                {{ 9, 10, 11, 12 }},
                {{ 13, 14, 15, 16 }}
                }});

            rtc::Matrix<4, 4> B({{
                {{ 1, 2, 3, 4 }},
                {{ 5, 6, 7, 8 }},
                {{ 9, 10, 11, 12 }},
                {{ 13, 14, 15, 16 }}
                }});

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
            rtc::Matrix<4, 4> A({{
                {{ 1, 2, 3, 4 }},
                {{ 5, 6, 7, 8 }},
                {{ 9, 10, 11, 12 }},
                {{ 13, 14, 15, 16 }}
                }});

            rtc::Matrix<4, 4> B({{
                {{ 1, 2, 3, 4 }},
                {{ 5, 6, 7, 8 }},
                {{ 9, 8, 7, 6 }},
                {{ 5, 4, 3, 2 }}
                }});

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
            rtc::Matrix44 A({{
                {{ 1, 2, 3, 4 }},
                {{ 5, 6, 7, 8 }},
                {{ 9, 8, 7, 6 }},
                {{ 5, 4, 3, 2 }}
                }});

            rtc::Matrix44 B({{
                {{ -2, 1, 2, 3 }},
                {{ 3, 2, 1, -1 }},
                {{ 4, 3, 6, 5 }},
                {{ 1, 2, 7, 8 }}
                }});

            rtc::Matrix44 expected({{
                {{ 20, 22, 50, 48 }},
                {{ 44, 54, 114, 108 }},
                {{ 40, 58, 110, 102 }},
                {{ 16, 26, 46, 42 }}
                }});

            THEN("A * B is { { 20, 22, 50, 48 }, { 44, 54, 114, 108 }, { 40, 58, 110, 102 }, { 16, 26, 46, 42 } }")
            {
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
            rtc::Matrix44 A({{
                {{ 1, 2, 3, 4 }},
                {{ 2, 4, 4, 2 }},
                {{ 8, 6, 4, 1 }},
                {{ 0, 0, 0, 1 }}
                }});

            rtc::Tuple b(1, 2, 3, 1);

            THEN("A * b = tuple(18, 24, 33, 1)")
            {
                REQUIRE(rtc::Tuple::Equal(rtc::Matrix44::Multiply(A, b), rtc::Tuple(18, 24, 33, 1)));
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
            rtc::Matrix44 A({{
                {{ 0, 1, 2, 4 }},
                {{ 1, 2, 4, 8 }},
                {{ 2, 4, 8, 16 }},
                {{ 4, 8, 16, 32 }}
                }});

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
        rtc::Tuple a(1, 2, 3, 4);

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
            rtc::Matrix44 A({{
                {{ 0, 9, 3, 0 }},
                {{ 9, 8, 0, 8 }},
                {{ 1, 8, 5, 3 }},
                {{ 0, 0, 5, 8 }}
                }});

            rtc::Matrix44 transpose({{
                {{ 0, 9, 1, 0 }},
                {{ 9, 8, 8, 0 }},
                {{ 3, 0, 5, 5 }},
                {{ 0, 8, 3, 8 }}
                }});

            THEN("Transpose(A) is { { 0, 9, 1, 0 }, { 9, 8, 8, 0 }, { 3, 0, 5, 5 }, { 0, 8, 3, 8 } }")
            {
                A.Transpose();

                REQUIRE(rtc::Matrix44::Equal(A, transpose));
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
            rtc::Matrix44 identity = rtc::Matrix44::Identity();
            rtc::Matrix44 A        = rtc::Matrix44::Transpose(identity);

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
            rtc::Matrix22 A({{ {{ 1, 5 }}, {{ -3, 2 }} }});

            THEN("determinant(A) = 17")
            {
                REQUIRE(rtc::Equal(A.Determinant(), 17));
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
            rtc::Matrix33 A({{ {{ 1, 5, 0 }}, {{ -3, 2, 7 }}, {{ 0, 6, -3 }} }});

            THEN("submatrix(A, 0, 2) is the following 2x2 matrix { { -3, 2 }, { 0, 6 } }")
            {
                rtc::Matrix22 expected({{ {{ -3, 2 }}, {{ 0, 6 }} }});

                REQUIRE(rtc::Matrix22::Equal(A.Submatrix(0, 2), expected));
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
            rtc::Matrix44 A({{
                {{ -6, 1, 1, 6 }},
                {{ -8, 5, 8, 6 }},
                {{ -1, 0, 8, 2 }},
                {{ -7, 1, -1, 1 }}
                }});

            THEN("submatrix(A, 2, 1) is the following 3x3 matrix { { -6, 1, 6 }, { -8, 8, 6 }, { -7, -1, 1 } }")
            {
                rtc::Matrix33 expected({{ {{ -6, 1, 6 }}, {{ -8, 8, 6 }}, {{ -7, -1, 1 }} }});

                REQUIRE(rtc::Matrix33::Equal(A.Submatrix(2, 1), expected));
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
            rtc::Matrix33 A({{
                {{ 3, 5, 0 }},
                {{ 2, -1, -7 }},
                {{ 6, -1, 5 }}
                }});

            THEN("determinant(B) = 25 and minor(A, 1, 0) = 25")
            {
                rtc::Matrix22 B = rtc::Matrix33::Submatrix(A, 1, 0);

                REQUIRE(rtc::Equal(B.Determinant(), 25));
                REQUIRE(rtc::Equal(A.Minor(1, 0), 25));
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
            rtc::Matrix33 A({{
                {{ 3, 5, 0 }},
                {{ 2, -1, -7 }},
                {{ 6, -1, 5 }}
                }});

            THEN("minor(A, 0, 0) = -12 and cofactor(A, 0, 0) = -12 and minor(A, 1, 0) = 25 and cofactor(A, 1, 0) = -25")
            {
                REQUIRE(rtc::Equal(A.Minor(0, 0), -12));
                REQUIRE(rtc::Equal(A.Cofactor(0, 0), -12));
                REQUIRE(rtc::Equal(rtc::Matrix33::Minor(A, 1, 0), 25));
                REQUIRE(rtc::Equal(rtc::Matrix33::Cofactor(A, 1, 0), -25));
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
            rtc::Matrix33 A({{
                {{ 1, 2, 6 }},
                {{ -5, 8, -4 }},
                {{ 2, 6, 4 }}
                }});

            THEN("cofactor(A, 0, 0) = 56 and cofactor(A, 0, 1) = 12 and cofactor(A, 0, 2) = -46 and determinant(A) = -196")
            {
                REQUIRE(rtc::Equal(A.Cofactor(0, 0), 56));
                REQUIRE(rtc::Equal(A.Cofactor(0, 1), 12));
                REQUIRE(rtc::Equal(rtc::Matrix33::Cofactor(A, 0, 2), -46));
                REQUIRE(rtc::Equal(A.Determinant(), -196));
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
            rtc::Matrix44 A({{
                {{ -2, -8, 3, 5 }},
                {{ -3, 1, 7, 3 }},
                {{ 1, 2, -9, 6 }},
                {{ -6, 7, 7, -9 }}
                }});

            THEN("cofactor(A, 0, 0) = 690 and cofactor(A, 0, 1) = 447 and cofactor(A, 0, 2) = 210 and cofactor(A, 0, 3) = 51 and determinant(A) = -4071")
            {
                REQUIRE(rtc::Equal(A.Cofactor(0, 0), 690));
                REQUIRE(rtc::Equal(A.Cofactor(0, 1), 447));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 0, 2), 210));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 0, 3), 51));
                REQUIRE(rtc::Equal(A.Determinant(), -4071));
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
            rtc::Matrix44 A({{
                {{ 6, 4, 4, 4 }},
                {{ 5, 5, 7, 6 }},
                {{ 4, -9, 3, -7 }},
                {{ 9, 1, 7, -6 }}
                }});

            THEN("determinant(A) = -2120 and A is invertible")
            {
                REQUIRE(rtc::Equal(rtc::Matrix44::Determinant(A), -2120));
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
            rtc::Matrix44 A({{
                {{ -4, 2, -2, -3 }},
                {{ 9, 6, 2, 6 }},
                {{ 0, -5, 1, -5 }},
                {{ 0, 0, 0, 0 }}
                }});

            THEN("determinant(A) = 0 and A is not invertible")
            {
                REQUIRE(rtc::Equal(rtc::Matrix44::Determinant(A), 0));
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
            rtc::Matrix44 A({{
                {{ -5, 2, 6, -8 }},
                {{ 1, -5, 1, 8 }},
                {{ 7, 7, -6, -7 }},
                {{ 1, -3, 7, 4 }}
                }});
            rtc::Matrix44 B = rtc::Matrix44::Inverse(A);

            THEN("determinant(A) = 532 and cofactor(A, 2, 3) = -160 and B[3, 2] = -160/532 and cofactor(A, 3, 2) = 105 and B[2, 3] = 105/532 and B is { { 0.21805, 0.45113, 0.24060, -0.04511 }, { -0.80827, -1.45677, -0.44361, 0.52068 }, { -0.07895, -0.22368, -0.05263, 0.19737 }, { -0.52256, -0.81391, -0.30075, 0.30639 } }")
            {
                rtc::Matrix44 expected({ {
                    {{ 0.21805, 0.45113, 0.24060, -0.04511 }},
                    {{ -0.80827, -1.45677, -0.44361, 0.52068 }},
                    {{ -0.07895, -0.22368, -0.05263, 0.19737 }},
                    {{  -0.52256, -0.81391, -0.30075, 0.30639 }}
                    }});

                REQUIRE(rtc::Equal(rtc::Matrix44::Determinant(A), 532));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 2, 3), -160));
                REQUIRE(rtc::Equal(B.Get(3, 2), -160.0 / 532.0));
                REQUIRE(rtc::Equal(rtc::Matrix44::Cofactor(A, 3, 2), 105));
                REQUIRE(rtc::Equal(B.Get(2, 3), 105.0 / 532.0));
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
            rtc::Matrix44 A({{
                {{ 8, -5, 9, 2 }},
                {{ 7, 5, 6, 1 }},
                {{ -6, 0, 9, 6 }},
                {{ -3, 0, -9, -4 }}
                }});

            THEN("inverse(A) is { { -0.15385, -0.15385, -0.28205, -0.53846 }, { -0.07692, 0.12308, 0.02564, 0.03077 }, { 0.35897, 0.35897, 0.43590, 0.92308 }, { -0.69231, -0.69231, -0.76923, -1.92308 } }")
            {
                rtc::Matrix44 expected({{
                    {{ -0.15385, -0.15385, -0.28205, -0.53846 }},
                    {{ -0.07692, 0.12308, 0.02564, 0.03077 }},
                    {{ 0.35897, 0.35897, 0.43590, 0.92308 }},
                    {{ -0.69231, -0.69231, -0.76923, -1.92308 }}
                    }});

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
            rtc::Matrix44 A({{
                {{ 9, 3, 0, 9 }},
                {{ -5, -2, -6, -3 }},
                {{ -4, 9, 6, 4 }},
                {{ -7, 6, 6, 2 }}
                }});

            THEN("inverse(A) is { { -0.04074, -0.07778, 0.14444, -0.22222 }, { -0.07778, 0.03333, 0.36667, -0.33333 }, { -0.02901, -0.14630, -0.10926, 0.12963 }, { 0.17778, 0.06667, -0.26667, 0.33333 } }")
            {
                rtc::Matrix44 expected({{
                    {{ -0.04074, -0.07778, 0.14444, -0.22222 }},
                    {{ -0.07778, 0.03333, 0.36667, -0.33333 }},
                    {{ -0.02901, -0.14630, -0.10926, 0.12963 }},
                    {{ 0.17778, 0.06667, -0.26667, 0.33333 }}
                    }});

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
            rtc::Matrix44 A({{
                {{ 3, -9, 7, 3 }},
                {{ 3, -8, 2, -9 }},
                {{ -4, 4, 4, 1 }},
                {{ -6, 5, -1, 1 }}
                }});
            rtc::Matrix44 B({{
                {{ 8, 2, 2, 2 }},
                {{ 3, -1, 7, 0 }},
                {{ 7, 0, 5, 4 }},
                {{ 6, -2, 0, 5 }}
                }});
            rtc::Matrix44 C = rtc::Matrix44::Multiply(A, B);

            THEN("C * inverse(B) = A")
            {
                REQUIRE(rtc::Matrix44::Equal(rtc::Matrix44::Multiply(C, rtc::Matrix44::Inverse(B)), A));
            }
        }
    }
}
