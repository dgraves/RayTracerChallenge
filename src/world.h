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

#include "intersections.h"
#include "point_light.h"
#include "ray.h"
#include "sphere.h"

#include <vector>

namespace rtc
{
    class World
    {
    public:
        using Lights  = std::vector<PointLight>;
        using Objects = std::vector<Sphere>;

    public:
        World()
        {
        }

        World(Lights&& lights, Objects&& objects) :
            lights_(std::move(lights)),
            objects_(std::move(objects))
        {
        }

        size_t GetLightCount() const { return lights_.size(); }

        size_t GetObjectCount() const { return objects_.size(); }

        const Lights& GetLights() const { return lights_; }

        const Objects& GetObjects() const { return objects_; }

        // TODO: Bounds checking for vectors.
        const PointLight& GetLight(size_t index) const { return lights_[index]; }

        const Sphere& GetObject(size_t index) const { return objects_[index]; }

        void SetLight(size_t index, const PointLight& light) { lights_[index] = light; }

        void SetLight(size_t index, PointLight&& light) { lights_[index] = std::move(light); }

        void SetObject(size_t index, const Sphere& object) { objects_[index] = object; }

        void SetObject(size_t index, Sphere&& object) { objects_[index] = std::move(object); }

        void InsertLight(const PointLight& light) { lights_.push_back(light); }

        void InsertLight(PointLight&& light) { lights_.emplace_back(std::move(light)); }

        void InsertObject(const Sphere& object) { objects_.push_back(object); }

        void InsertObject(Sphere&& object) { objects_.emplace_back(std::move(object)); }

        Intersections Intersect(const Ray& ray) const;

        static World GetDefault();

    private:
        Lights  lights_;    ///< List of lights in the world.
        Objects objects_;   ///< List of objects in the world.
    };
}
