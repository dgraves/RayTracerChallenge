/*
** Copyright(c) 2021 Dustin Graves
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
#include "material.h"
#include "matrix44.h"
#include "point.h"
#include "ray.h"
#include "vector.h"

#include <memory>

namespace rtc
{
    class Shape : public std::enable_shared_from_this<Shape>
    {
    public:
        virtual ~Shape() = default;

        const Material& GetMaterial() const { return material_; }

        void SetMaterial(const Material& material) { material_ = material; }

        void SetMaterial(Material&& material) { material_ = std::move(material); }

        const Matrix44& GetTransform() const { return transform_; }

        const Matrix44& GetInverseTransform() const { return inverse_transform_; }

        const Matrix44& GetTransposedInverseTransform() const { return transposed_inverse_transform_; }

        void SetTransform(const Matrix44& transform)
        {
            transform_ = transform;
            ComputeInverseTransforms();
        }

        void SetTransform(Matrix44&& transform)
        {
            transform_ = std::move(transform);
            ComputeInverseTransforms();
        }

        void Intersect(const Ray& ray, Intersections::Values& values) const
        {
            // Transform ray to the shape's object space.
            const auto local_ray = Matrix44::Transform(ray, inverse_transform_);
            LocalIntersect(local_ray, values);
        }

        Intersections Intersect(const Ray& ray) const
        {
            Intersections::Values values{};
            Intersect(ray, values);
            return Intersections{ std::move(values) };
        }

        Vector NormalAt(const Point& world_point) const
        {
            // Convert from world space to object space to compute the normal as the vector
            // between the point and the center of the shape.
            const auto local_point  = Point{ Matrix44::Multiply(inverse_transform_, world_point) };
            const auto local_normal = LocalNormalAt(local_point);
            auto       world_normal = Vector{ Matrix44::Multiply(transposed_inverse_transform_, local_normal) };
            // The world_normal w component does not need to be set to 0 because the bottom row of the matrix was
            // cleared when it was computed by ComputeInverseTransform().
            world_normal.Normalize();
            return world_normal;
        }

    protected:
        Shape() :
            transform_(Matrix44::Identity()),
            inverse_transform_(Matrix44::Identity()),
            transposed_inverse_transform_(Matrix44::Identity())
        {
        }

        Shape(const Material& material) :
            material_(material),
            transform_(Matrix44::Identity()),
            inverse_transform_(Matrix44::Identity()),
            transposed_inverse_transform_(Matrix44::Identity())
        {
        }

        Shape(Material&& material) :
            material_(std::move(material)),
            transform_(Matrix44::Identity()),
            inverse_transform_(Matrix44::Identity()),
            transposed_inverse_transform_(Matrix44::Identity())
        {
        }

        Shape(const Matrix44& transform) :
            transform_(transform)
        {
            ComputeInverseTransforms();
        }

        Shape(Matrix44&& transform) :
            transform_(std::move(transform))
        {
            ComputeInverseTransforms();
        }

        Shape(const Material& material, const Matrix44& transform) :
            material_(material),
            transform_(transform)
        {
            ComputeInverseTransforms();
        }

        Shape(Material&& material, Matrix44&& transform) :
            material_(std::move(material)),
            transform_(std::move(transform))
        {
            ComputeInverseTransforms();
        }

    private:
        void ComputeInverseTransforms()
        {
            inverse_transform_            = Matrix44::Inverse(transform_);
            transposed_inverse_transform_ = Matrix44::Transpose(inverse_transform_);

            // If the original transform included translation, the normal computed with the
            // transpose of the inverse transform could end up with a non-zero w component.
            // The bottom row is cleared to avoid this.
            transposed_inverse_transform_.Set(3u, 0u, 0.0);
            transposed_inverse_transform_.Set(3u, 1u, 0.0);
            transposed_inverse_transform_.Set(3u, 2u, 0.0);
        }

        virtual void LocalIntersect(const Ray& local_ray, Intersections::Values& values) const = 0;

        virtual Vector LocalNormalAt(const Point& local_point) const = 0;

    private:
        Material material_;                      ///< Material properties describing how the sphere shoule be shaded.
        Matrix44 transform_;                     ///< Transform to determine the shape and position of the sphere.
        Matrix44 inverse_transform_;             ///< Inverse of the transform to be applied to rays for intersection testing.
        Matrix44 transposed_inverse_transform_;  ///< Transpose of the inverse of the transform for surface normal calculation.
    };
}
