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

#include "camera.h"
#include "canvas.h"
#include "color.h"
#include "double_util.h"
#include "intersections.h"
#include "material.h"
#include "phong.h"
#include "plane.h"
#include "point.h"
#include "point_light.h"
#include "ppm_writer.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"
#include "world.h"

#include <string>

// Render the silhouette of a sphere (a circle), from Chapter 5 "Putting it together".
void RenderSphereSilhouette(const std::string& filename)
{
    // Parameters for a 'wall' to project the surface on and the render canvas.
    const auto wall_z        = 10.0;
    const auto wall_size     = 7.0;
    const auto canvas_pixels = 100u;
    const auto pixel_size    = wall_size / static_cast<double>(canvas_pixels);
    const auto half          = wall_size / 2.0;

    auto       canvas     = rtc::Canvas{ canvas_pixels, canvas_pixels };
    const auto color      = rtc::Color{ 1.0, 0.0, 0.0 }; // Red
    const auto ray_origin = rtc::Point{ 0.0, 0.0, -5.0 };
    const auto shape      = rtc::Sphere::Create();

    for (auto y = 0u; y < canvas_pixels; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half).
        const auto world_y = half - pixel_size * static_cast<double>(y);

        for (auto x = 0u; x < canvas_pixels; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half).
            const auto world_x = -half + pixel_size * static_cast<double>(x);

            // Describe the point on the wall that the ray will target.
            const auto position = rtc::Point(world_x, world_y, wall_z);

            const auto r  = rtc::Ray{ ray_origin, rtc::Vector::Normalize(rtc::Point::Subtract(position, ray_origin)) };
            const auto xs = shape->Intersect(r);

            if (xs.Hit() != nullptr)
            {
                canvas.WritePixel(x, y, color);
            }
        }
    }

    rtc::PpmWriter::WriteFile(filename, canvas);
}

// Render a sphere, from Chapter 6 "Putting it together".
void RenderSphere(const std::string& filename)
{
    // Parameters for a 'wall' to project the surface on and the render canvas.
    const auto wall_z        = 10.0;
    const auto wall_size     = 7.0;
    const auto canvas_pixels = 500u;
    const auto pixel_size    = wall_size / static_cast<double>(canvas_pixels);
    const auto half          = wall_size / 2.0;

    auto       canvas     = rtc::Canvas{ canvas_pixels, canvas_pixels };
    const auto ray_origin = rtc::Point{ 0.0, 0.0, -5.0 };
    auto       shape      = rtc::Sphere::Create();
    auto       material   = rtc::Material{};
    const auto light      = rtc::PointLight{ rtc::Point{ -10.0, 10.0, -10.0 }, rtc::Color{ 1.0, 1.0, 1.0 } };

    material.SetColor(rtc::Color{ 1.0, 0.2, 1.0 });
    shape->SetMaterial(material);

    for (auto y = 0u; y < canvas_pixels; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half).
        const auto world_y = half - pixel_size * static_cast<double>(y);

        for (auto x = 0u; x < canvas_pixels; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half).
            const auto world_x = -half + pixel_size * static_cast<double>(x);

            // Describe the point on the wall that the ray will target.
            const auto position = rtc::Point(world_x, world_y, wall_z);

            const auto r   = rtc::Ray(ray_origin, rtc::Vector::Normalize(rtc::Point::Subtract(position, ray_origin)));
            const auto xs  = shape->Intersect(r);
            const auto hit = xs.Hit();

            if (hit != nullptr)
            {
                const auto object = hit->GetObject();
                const auto point  = r.GetPosition(hit->GetT());
                const auto normal = object->NormalAt(point);
                const auto eye    = rtc::Vector::Negate(r.GetDirection());
                const auto color  = rtc::Phong::Lighting(object->GetMaterial(), light, point, eye, normal, false);

                canvas.WritePixel(x, y, color);
            }
        }
    }

    rtc::PpmWriter::WriteFile(filename, canvas);
}

// Render a scene, from Chapter 7 "Making a scene".
void RenderScene(const std::string& filename)
{
    const auto floor_material = rtc::Material{
        rtc::Color{ 1.0, 0.9, 0.9 },
        rtc::Material::GetDefaultAmbient(),
        rtc::Material::GetDefaultDiffuse(),
        0.0,
        rtc::Material::GetDefaultShininess() };

    const auto world = rtc::World{
        {
            rtc::PointLight{ rtc::Point{ -10.0, 10.0, -10.0 }, rtc::Color{ 1.0, 1.0, 1.0 } }
        },
        {
            // Parameters for the floor, constructed from an extremely flattened sphere with a matte texture.
            rtc::Sphere::Create(rtc::Material{ floor_material }, rtc::Matrix44::Scaling(10.0, 0.01, 10.0)),

            // Parameters for the left wall, with the same scale and color as the floor, but rotated and translated into place.
            rtc::Sphere::Create(
                rtc::Material{ floor_material },
                rtc::Matrix44::Multiply(
                    rtc::Matrix44::Translation(0.0, 0.0, 5.0),
                    rtc::Matrix44::RotationY(-rtc::kPi / 4.0),
                    rtc::Matrix44::RotationX(rtc::kPi / 2.0),
                    rtc::Matrix44::Scaling(10.0, 0.01, 10.0))),

            // Parameters for the right wall, which is identical to the left, but rotated the opposite direction in y.
            rtc::Sphere::Create(
                rtc::Material{ floor_material },
                rtc::Matrix44::Multiply(
                    rtc::Matrix44::Translation(0.0, 0.0, 5.0),
                    rtc::Matrix44::RotationY(rtc::kPi / 4.0),
                    rtc::Matrix44::RotationX(rtc::kPi / 2.0),
                    rtc::Matrix44::Scaling(10.0, 0.01, 10.0))),

            // Parameters for the large sphere in the middle, which is a unit sphere, translated upward slightly and colored green.
            rtc::Sphere::Create(
                rtc::Material{
                    rtc::Color{ 0.1, 1.0, 0.5 },
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess() },
                rtc::Matrix44::Translation(-0.5, 1.0, 0.5)),

            // Parameters for the smaller green sphere on the right, which is scaled by half.
            rtc::Sphere::Create(
                rtc::Material{
                    rtc::Color{ 0.5, 1.0, 0.1 },
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess() },
                rtc::Matrix44::Multiply(rtc::Matrix44::Translation(1.5, 0.5, -0.5), rtc::Matrix44::Scaling(0.5, 0.5, 0.5))),

            // Parameters for the smallest sphere, which is scaled by a third before being translated.
            rtc::Sphere::Create(
                rtc::Material{
                    rtc::Color{ 1.0, 0.8, 0.1 },
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess() },
                rtc::Matrix44::Multiply(rtc::Matrix44::Translation(-1.5, 0.33, -0.75), rtc::Matrix44::Scaling(0.33, 0.33, 0.33)))
        } };

    // Construct the camera and render the world.
    const auto from   = rtc::Point{ 0.0, 1.5, -5.0 };
    const auto to     = rtc::Point{ 0.0, 1.0, 0.0 };
    const auto up     = rtc::Vector{ 0.0, 1.0, 0.0 };
    const auto camera = rtc::Camera{ 1000u, 500u, rtc::kPi / 3.0, rtc::Matrix44::ViewTransform(from, to, up) };
    const auto canvas = camera.Render(world);

    rtc::PpmWriter::WriteFile(filename, canvas);
}

// Render a scene with a plane, from Chapter 9 "Planes".
void RenderPlaneScene(const std::string& filename)
{
    const auto world = rtc::World{
        {
            rtc::PointLight{ rtc::Point{ -10.0, 10.0, -10.0 }, rtc::Color{ 1.0, 1.0, 1.0 } }
        },
        {
            // Parameters for the floor, constructed from a plane with a matte texture.
            rtc::Plane::Create(
                rtc::Material{
                    rtc::Color{ 1.0, 0.9, 0.9 },
                    rtc::Material::GetDefaultAmbient(),
                    rtc::Material::GetDefaultDiffuse(),
                    0.0,
                    rtc::Material::GetDefaultShininess() }),

            // Parameters for the large sphere in the middle, which is a unit sphere, translated upward slightly and colored green.
            rtc::Sphere::Create(
                rtc::Material{
                    rtc::Color{ 0.1, 1.0, 0.5 },
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess() },
                rtc::Matrix44::Translation(-0.5, 1.0, 0.5)),

            // Parameters for the smaller green sphere on the right, which is scaled by half.
            rtc::Sphere::Create(
                rtc::Material{
                    rtc::Color{ 0.5, 1.0, 0.1 },
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess() },
                rtc::Matrix44::Multiply(rtc::Matrix44::Translation(1.5, 0.5, -0.5), rtc::Matrix44::Scaling(0.5, 0.5, 0.5))),

                // Parameters for the smallest sphere, which is scaled by a third before being translated.
                rtc::Sphere::Create(
                    rtc::Material{
                        rtc::Color{ 1.0, 0.8, 0.1 },
                        rtc::Material::GetDefaultAmbient(),
                        0.7,
                        0.3,
                        rtc::Material::GetDefaultShininess() },
                    rtc::Matrix44::Multiply(rtc::Matrix44::Translation(-1.5, 0.33, -0.75), rtc::Matrix44::Scaling(0.33, 0.33, 0.33)))
            } };

    // Construct the camera and render the world.
    const auto from = rtc::Point{ 0.0, 1.5, -5.0 };
    const auto to = rtc::Point{ 0.0, 1.0, 0.0 };
    const auto up = rtc::Vector{ 0.0, 1.0, 0.0 };
    const auto camera = rtc::Camera{ 1000u, 500u, rtc::kPi / 3.0, rtc::Matrix44::ViewTransform(from, to, up) };
    const auto canvas = camera.Render(world);

    rtc::PpmWriter::WriteFile(filename, canvas);
}

int main()
{
    RenderSphereSilhouette("silhouette.ppm");

    RenderSphere("sphere.ppm");

    RenderScene("scene.ppm");

    RenderPlaneScene("plane.ppm");

    return 0;
}
