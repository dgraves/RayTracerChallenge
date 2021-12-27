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

#include "ray.h"
#include "sphere.h"
#include "world.h"

#include <vector>

namespace rtc
{
    class Intersect
    {
    public:
        struct Intersection
        {
            double        t{ 0.0 };           ///< Value representing intersection 'time'.
            const Sphere* object{ nullptr };  ///< Pointer to intersected object.
        };

        using Intersections = std::vector<Intersection>;

    public:
        Intersect() : hit_(nullptr) { }

        Intersect(const Sphere* sphere, const Ray& ray) : hit_(nullptr) { TestIntersect(sphere, ray); }

        Intersect(const World& world, const Ray& ray) : hit_(nullptr) { TestIntersect(world, ray); }

        size_t GetCount() const { return values_.size(); }

        const Intersection& GetValue(size_t index) const { return values_[index]; }

        void Test(const Sphere* sphere, const Ray& ray) { Reset(); TestIntersect(sphere, ray); }

        void Test(const World& world, const Ray& ray) { Reset(); TestIntersect(world, ray); }

        const Intersection* Hit()
        {
            if (hit_ == nullptr)
            {
                hit_ = Hit(values_);
            }

            return hit_;
        }

        static void Sort(Intersections& intersections);

        static const Intersection* Hit(const Intersections& intersections);

    private:
        void Reset() { hit_ = nullptr; values_.clear(); }

        void TestIntersect(const Sphere* sphere, const Ray& ray);

        void TestIntersect(const World& world, const Ray& ray);

    private:
        Intersections       values_;    ///< Values representing intersection 'times'.
        const Intersection* hit_;       ///< Cached hit value, set by the first call to Hit().
    };
}
