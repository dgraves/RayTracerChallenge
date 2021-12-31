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

#pragma once

#include "intersection.h"
#include "ray.h"

#include <memory>
#include <vector>

namespace rtc
{
    class Intersections
    {
    public:
        using Values = std::vector<Intersection>;

    public:
        Intersections(const Values& values) :
            values_(values)
        {
        }

        Intersections(Values&& values) :
            values_(std::move(values))
        {
        }

        Intersections(const Values& values, bool sort) :
            values_(values)
        {
            if (sort)
            {
                Sort();
            }
        }

        Intersections(Values&& values, bool sort) :
            values_(std::move(values))
        {
            if (sort)
            {
                Sort();
            }
        }

        bool IsEmpty() const { return values_.empty(); }

        size_t GetCount() const { return values_.size(); }

        const Intersection& GetValue(size_t index) const { return values_.at(index); }

        const Values& GetValues() const { return values_; }

        void Sort() { Sort(values_); }

        const Intersection* Hit() const { return Hit(values_); }

        static void Sort(Intersections::Values& intersections);

        static const Intersection* Hit(const Intersections::Values& intersections);

    private:
        Values values_;    ///< Values representing intersection 'times'.
    };
}
