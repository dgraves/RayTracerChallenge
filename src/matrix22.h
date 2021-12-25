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

#pragma once

#include "matrix.h"

namespace rtc
{
    class Matrix22 : public Matrix<2, 2>
    {
    public:
        Matrix22()
        {
        }

        Matrix22(const Matrix& matrix) :
            Matrix(matrix)
        {
        }

        Matrix22(Matrix&& matrix) :
            Matrix(std::move(matrix))
        {
        }

        Matrix22(double data[2][2]) :
            Matrix(data)
        {
        }

        Matrix22(std::array<std::array<double, 2>, 2>&& data) :
            Matrix(std::move(data))
        {
        }

        static Matrix22 Identity()
        {
            return Matrix22({{
                    {{ 1, 0 }},
                    {{ 0, 1 }}
                    }});
        }
    };
}
