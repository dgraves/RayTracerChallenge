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
** The above copyright noticeand this permission notice shall be included in all
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

#include "material.h"
#include "matrix44.h"
#include "point.h"
#include "vector.h"

namespace rtc
{
    class Sphere
    {
    public:
        // Default construct a unit sphere.
        Sphere() :
            transform_(rtc::Matrix44::Identity()),
            inverse_transform_(rtc::Matrix44::Identity()),
            transpose_inverse_transform_(rtc::Matrix44::Identity())
        {
        }

        Sphere(const Material& material) :
            material_(material),
            transform_(rtc::Matrix44::Identity()),
            inverse_transform_(rtc::Matrix44::Identity()),
            transpose_inverse_transform_(rtc::Matrix44::Identity())
        {
        }

        Sphere(Material&& material) :
            material_(std::move(material)),
            transform_(rtc::Matrix44::Identity()),
            inverse_transform_(rtc::Matrix44::Identity()),
            transpose_inverse_transform_(rtc::Matrix44::Identity())
        {
        }

        Sphere(const Matrix44& transform) :
            transform_(transform)
        {
            ComputeInverseTransforms();
        }

        Sphere(Matrix44&& transform) :
            transform_(std::move(transform))
        {
            ComputeInverseTransforms();
        }

        Sphere(const Material& material, const Matrix44& transform) :
            material_(material),
            transform_(transform)
        {
            ComputeInverseTransforms();
        }

        Sphere(Material&& material, Matrix44&& transform) :
            material_(std::move(material)),
            transform_(std::move(transform))
        {
            ComputeInverseTransforms();
        }

        const Material& GetMaterial() const { return material_; }

        void SetMaterial(const Material& material) { material_ = material; }

        const Matrix44& GetTransform() const { return transform_; }

        const Matrix44& GetInverseTransform() const { return inverse_transform_; }

        void SetTransform(const Matrix44& transform)
        {
            transform_ = transform;
            ComputeInverseTransforms();
        }

        Vector NormalAt(const Point& world_point) const
        {
            // Convert from world space to object space to compute the normal as the vector
            // between the point and the center of the sphere.
            rtc::Point  object_point(rtc::Matrix44::Multiply(inverse_transform_, world_point));
            rtc::Vector object_normal(object_point);
            rtc::Vector world_normal(rtc::Matrix44::Multiply(transpose_inverse_transform_, object_normal));
            world_normal.Normalize();
            return world_normal;
        }

        static bool Equal(const Sphere& lhs, const Sphere& rhs)
        {
            return (Material::Equal(lhs.material_, rhs.material_) &&
                    Matrix44::Equal(lhs.transform_, rhs.transform_));
        }

    private:
        void ComputeInverseTransforms()
        {
            inverse_transform_ = rtc::Matrix44::Inverse(transform_);
            transpose_inverse_transform_ = rtc::Matrix44::Transpose(inverse_transform_);

            // If the original transform included translation, the normal computed with the
            // transpose of the inverse transform could end up with a non-zero w component.
            // The bottom row is cleared to avoid this.
            transpose_inverse_transform_.Set(3, 0, 0.0);
            transpose_inverse_transform_.Set(3, 1, 0.0);
            transpose_inverse_transform_.Set(3, 2, 0.0);
        }

    private:
        Material material_;                     ///< Material properties describing how the sphere shoule be shaded.
        Matrix44 transform_;                    ///< Transform to determine the shape and position of the sphere.
        Matrix44 inverse_transform_;            ///< Inverse of the transform to be applied to rays for intersection testing.
        Matrix44 transpose_inverse_transform_;  ///< Transpose of the inverse of the transform for surface normal calculation.
    };
}
