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
#include "intersect.h"
#include "material.h"
#include "phong.h"
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
    double wall_z        = 10.0;
    double wall_size     = 7.0;
    double canvas_pixels = 100.0;
    double pixel_size    = wall_size / canvas_pixels;
    double half          = wall_size / 2.0;

    auto canvas     = rtc::Canvas(canvas_pixels, canvas_pixels);
    auto color      = rtc::Color(1, 0, 0); // Red
    auto ray_origin = rtc::Point(0, 0, -5);
    auto shape      = rtc::Sphere();

    for (uint32_t y = 0; y < canvas_pixels; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half).
        double world_y = half - pixel_size * y;

        for (uint32_t x = 0; x < canvas_pixels; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half).
            double world_x = -half + pixel_size * x;

            // Describe the point on the wall that the ray will target.
            auto position = rtc::Point(world_x, world_y, wall_z);

            auto r  = rtc::Ray(ray_origin, rtc::Vector::Normalize(rtc::Point::Subtract(position, ray_origin)));
            auto xs = rtc::Intersect(&shape, r);

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
    double wall_z        = 10.0;
    double wall_size     = 7.0;
    double canvas_pixels = 500.0;
    double pixel_size    = wall_size / canvas_pixels;
    double half          = wall_size / 2.0;

    auto canvas     = rtc::Canvas(canvas_pixels, canvas_pixels);
    auto ray_origin = rtc::Point(0, 0, -5);
    auto shape      = rtc::Sphere();
    auto material   = rtc::Material();
    auto light      = rtc::PointLight(rtc::Point(-10, 10, -10), rtc::Color(1, 1, 1));

    material.SetColor(rtc::Color(1, 0.2, 1));
    shape.SetMaterial(material);

    for (uint32_t y = 0; y < canvas_pixels; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half).
        double world_y = half - pixel_size * y;

        for (uint32_t x = 0; x < canvas_pixels; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half).
            double world_x = -half + pixel_size * x;

            // Describe the point on the wall that the ray will target.
            auto position = rtc::Point(world_x, world_y, wall_z);

            auto r   = rtc::Ray(ray_origin, rtc::Vector::Normalize(rtc::Point::Subtract(position, ray_origin)));
            auto xs  = rtc::Intersect(&shape, r);
            auto hit = xs.Hit();

            if (hit != nullptr)
            {
                auto object = hit->object;
                auto point  = r.GetPosition(hit->t);
                auto normal = object->NormalAt(point);
                auto eye    = rtc::Vector::Negate(r.GetDirection());
                auto color  = rtc::Phong::Lighting(object->GetMaterial(), light, point, eye, normal);

                canvas.WritePixel(x, y, color);
            }
        }
    }

    rtc::PpmWriter::WriteFile(filename, canvas);
}

// Render a scene, from Chapter 7 "Making a scene".
void RenderScene(const std::string& filename)
{
    auto floor_material = rtc::Material(
        rtc::Color(1.0, 0.9, 0.9),
        rtc::Material::GetDefaultAmbient(),
        rtc::Material::GetDefaultDiffuse(),
        0.0,
        rtc::Material::GetDefaultShininess());

    auto world = rtc::World(
        {
            rtc::PointLight(rtc::Point(-10.0, 10.0, -10.0), rtc::Color(1.0, 1.0, 1.0))
        },
        {
            // Parameters for the floor, constructed from an extremely flattened sphere with a matte texture.
            rtc::Sphere(floor_material, rtc::Matrix44::Scaling(10.0, 0.01, 10.0)),

            // Parameters for the left wall, with the same scale and color as the floor, but rotated and translated into place.
            rtc::Sphere(
                floor_material,
                rtc::Matrix44::Multiply(
                    rtc::Matrix44::Multiply(
                        rtc::Matrix44::Multiply(
                            rtc::Matrix44::Translation(0.0, 0.0, 5.0),
                            rtc::Matrix44::RotationY(-rtc::kPi / 4.0)),
                        rtc::Matrix44::RotationX(rtc::kPi / 2.0)),
                    rtc::Matrix44::Scaling(10.0, 0.01, 10.0))),

            // Parameters for the right wall, which is identical to the left, but rotated the opposite direction in y.
            rtc::Sphere(
                floor_material,
                rtc::Matrix44::Multiply(
                    rtc::Matrix44::Multiply(
                        rtc::Matrix44::Multiply(
                            rtc::Matrix44::Translation(0.0, 0.0, 5.0),
                            rtc::Matrix44::RotationY(rtc::kPi / 4.0)),
                        rtc::Matrix44::RotationX(rtc::kPi / 2.0)),
                    rtc::Matrix44::Scaling(10.0, 0.01, 10.0))),

            // Parameters for the large sphere in the middle, which is a unit sphere, translated upward slightly and colored green.
            rtc::Sphere(
                rtc::Material(
                    rtc::Color(0.1, 1.0, 0.5),
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess()),
                rtc::Matrix44::Translation(-0.5, 1.0, 0.5)),

            // Parameters for the smaller green sphere on the right, which is scaled by half.
            rtc::Sphere(
                rtc::Material(
                    rtc::Color(0.5, 1.0, 0.1),
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess()),
                rtc::Matrix44::Multiply(rtc::Matrix44::Translation(1.5, 0.5, -0.5), rtc::Matrix44::Scaling(0.5, 0.5, 0.5))),

            // Parameters for the smallest sphere, which is scaled by a third before being translated.
            rtc::Sphere(
                rtc::Material(
                    rtc::Color(1.0, 0.8, 0.1),
                    rtc::Material::GetDefaultAmbient(),
                    0.7,
                    0.3,
                    rtc::Material::GetDefaultShininess()),
                rtc::Matrix44::Multiply(rtc::Matrix44::Translation(-1.5, 0.33, -0.75), rtc::Matrix44::Scaling(0.33, 0.33, 0.33)))
        });

    // Construct the camera and render the world.
    auto from   = rtc::Point(0.0, 1.5, -5.0);
    auto to     = rtc::Point(0.0, 1.0, 0.0);
    auto up     = rtc::Vector(0.0, 1.0, 0.0);
    auto camera = rtc::Camera(1000u, 500u, rtc::kPi / 3.0, rtc::Matrix44::ViewTransform(from, to, up));
    auto canvas = camera.Render(world);

    rtc::PpmWriter::WriteFile(filename, canvas);
}

int main()
{
    RenderSphereSilhouette("silhouette.ppm");

    RenderSphere("sphere.ppm");

    RenderScene("scene.ppm");

    return 0;
}
