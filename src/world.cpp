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

#include "world.h"

#include "color.h"
#include "material.h"
#include "matrix44.h"
#include "point.h"
#include "sphere.h"

#include <cassert>

namespace rtc
{
    Intersections World::Intersect(const Ray& ray) const
    {
        Intersections::Values values{};

        for (const auto& object : objects_)
        {
            assert(object && "World::Intersect attempted to process an invalid object");
            object->Intersect(ray, values);
        }

        return Intersections{ std::move(values), true };
    }

    World World::GetDefault()
    {
        return World{
            {
                PointLight{ rtc::Point{ -10.0, 10.0, -10.0 }, rtc::Color{ 1.0, 1.0, 1.0} }
            },
            {
                Sphere::Create(
                    rtc::Material{
                        rtc::Color{ 0.8, 1.0, 0.6 },
                        rtc::Material::GetDefaultAmbient(),
                        0.7,
                        0.2,
                        rtc::Material::GetDefaultShininess() }),
                Sphere::Create(rtc::Matrix44::Scaling(0.5, 0.5, 0.5))
            } };
    }
}
