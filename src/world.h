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
#include "shape.h"

#include <memory>
#include <vector>

namespace rtc
{
    class World
    {
    public:
        using Lights  = std::vector<PointLight>;
        using Objects = std::vector<std::shared_ptr<Shape>>;

    public:
        World() = default;

        World(Lights&& lights, Objects&& objects) :
            lights_(std::move(lights)),
            objects_(std::move(objects))
        {
        }

        size_t GetLightCount() const { return lights_.size(); }

        size_t GetObjectCount() const { return objects_.size(); }

        const Lights& GetLights() const { return lights_; }

        const Objects& GetObjects() const { return objects_; }

        const PointLight& GetLight(size_t index) const { return lights_.at(index); }

        const std::shared_ptr<Shape>& GetObject(size_t index) const { return objects_.at(index); }

        void SetLight(size_t index, const PointLight& light) { lights_.at(index) = light; }

        void SetLight(size_t index, PointLight&& light) { lights_.at(index) = std::move(light); }

        void SetObject(size_t index, const std::shared_ptr<Shape>& object) { objects_.at(index) = object; }

        void SetObject(size_t index, std::shared_ptr<Shape>&& object) { objects_.at(index) = std::move(object); }

        void AppendLight(const PointLight& light) { lights_.push_back(light); }

        void AppendLight(PointLight&& light) { lights_.emplace_back(std::move(light)); }

        void AppendObject(const std::shared_ptr<Shape>& object) { objects_.push_back(object); }

        void AppendObject(std::shared_ptr<Shape>&& object) { objects_.emplace_back(std::move(object)); }

        Intersections Intersect(const Ray& ray) const;

        static World GetDefault();

    private:
        Lights  lights_;    ///< List of lights in the world.
        Objects objects_;   ///< List of objects in the world.
    };
}
