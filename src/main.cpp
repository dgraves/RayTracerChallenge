/*
** Copyright(c) 2020 Dustin Graves
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

#include "canvas.h"
#include "color.h"
#include "intersect.h"
#include "point.h"
#include "ppm_writer.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"

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

    rtc::Canvas canvas(canvas_pixels, canvas_pixels);
    rtc::Color  color(1, 0, 0); // Red
    rtc::Point  ray_origin(0, 0, -5);
    rtc::Sphere shape;

    for (uint32_t y = 0; y < canvas_pixels; ++y)
    {
        // Compute the world y coordinate (top = +half, bottom = -half).
        double world_y = half - pixel_size * y;

        for (uint32_t x = 0; x < canvas_pixels; ++x)
        {
            // Compute the world x coordinate (left = -half, right = +half).
            double world_x = -half + pixel_size * x;

            // Describe the point on the wall that the ray will target.
            rtc::Point position(world_x, world_y, wall_z);

            rtc::Ray       r(ray_origin, rtc::Vector::Normalize(rtc::Point::Subtract(position, ray_origin)));
            rtc::Intersect xs(&shape, r);

            if (xs.Hit() != nullptr)
            {
                canvas.WritePixel(x, y, color);
            }
        }
    }

    rtc::PpmWriter::WriteFile(filename, canvas);
}

int main()
{
    RenderSphereSilhouette("silhouette.ppm");

    return 0;
}
