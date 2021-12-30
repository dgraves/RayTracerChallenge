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

#include "intersections.h"

#include <algorithm>

namespace rtc
{
    void Intersections::Sort(Intersections::Values& values)
    {
        // Maintain original ordering of intersections with the same t value.
        std::stable_sort(values.begin(), values.end(), [](const Intersections::Intersection& lhs, const Intersections::Intersection& rhs)
            {
                return (lhs.t < rhs.t);
            });
    }

    const Intersections::Intersection* Intersections::Hit(const Intersections::Values& values)
    {
        const Intersections::Intersection* hit = nullptr;

        // Find the first non-negative value.  Because hits are expected to be sorted by t,
        // this is also the first hit.
        for (const auto& value : values)
        {
            if (value.t >= 0)
            {
                hit = &value;
                break;
            }
        }

        return hit;
    }
}
