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

#include "phong.h"

#include <cmath>

namespace rtc
{
    Color Phong::Lighting(const Material& material, const PointLight& light, const Point& point, const Vector& eyev, const Vector& normalv)
    {
        // Combine the surface color with the light's color/intensity.
        Color effective_color = Color::HadamardProduct(material.GetColor(), light.GetIntensity());

        // Find the direction to the light source.
        Vector lightv = Vector::Subtract(light.GetPosition(), point);
        lightv.Normalize();

        // Compute the ambient contribution.
        Color ambient = Color::Multiply(effective_color, material.GetAmbient());

        // The light_dot_normal value represents the cosine of the angle between the
        // light vector and the normal vector.  A negative number means the light is
        // on the other side of the surface.
        double light_dot_normal = Vector::Dot(lightv, normalv);

        Color diffuse;
        Color specular;

        if (light_dot_normal >= 0)
        {
            // Compute the diffuse contribution.
            diffuse = Color::Multiply(effective_color, (material.GetDiffuse() * light_dot_normal));

            // The reflect_dot_eye value represents the cosine of the angle between the
            // reflection vector and the eye vector.  A negative number means the light
            // reflects away from the eye.
            Vector reflectv        = Vector::Reflect(Vector::Negate(lightv), normalv);
            double reflect_dot_eye = Vector::Dot(reflectv, eyev);

            if (reflect_dot_eye > 0)
            {
                // Compute the specular contribution.
                double factor = pow(reflect_dot_eye, material.GetShininess());

                specular = Vector::Multiply(light.GetIntensity(), (material.GetSpecular() * factor));
            }
        }

        // Add the three contributions together to get the final shading.
        return Color::Add(ambient, Color::Add(diffuse, specular));
    }
}
