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

#include "color.h"
#include "double_util.h"

namespace rtc
{
    class Material
    {
    public:
        Material() :
            color_(GetDefaultColor()),
            ambient_(GetDefaultAmbient()),
            diffuse_(GetDefaultDiffuse()),
            specular_(GetDefaultSpecular()),
            shininess_(GetDefaultShininess())
        {
        }

        Material(const Color& color, double ambient, double diffuse, double specular, double shininess) :
            color_(color),
            ambient_(ambient),
            diffuse_(diffuse),
            specular_(specular),
            shininess_(shininess)
        {
        }

        Material(Color&& color, double ambient, double diffuse, double specular, double shininess) :
            color_(std::move(color)),
            ambient_(ambient),
            diffuse_(diffuse),
            specular_(specular),
            shininess_(shininess)
        {
        }

        const Color& GetColor() const { return color_; }

        double GetAmbient() const { return ambient_; }

        double GetDiffuse() const { return diffuse_; }

        double GetSpecular() const { return specular_; }

        double GetShininess() const { return shininess_; }

        void SetColor(const Color& color) { color_ = color; }

        void SetAmbient(double ambient) { ambient_ = ambient; }

        void SetDiffuse(double diffuse) { diffuse_ = diffuse; }

        void SetSpecular(double specular) { specular_ = specular; }

        void SetShininess(double shininess) { shininess_ = shininess; }

        static bool Equal(const Material& lhs, const Material& rhs)
        {
            return (rtc::Color::Equal(lhs.color_, rhs.color_) &&
                    rtc::Equal(lhs.ambient_, rhs.ambient_) &&
                    rtc::Equal(lhs.diffuse_, rhs.diffuse_) &&
                    rtc::Equal(lhs.specular_, rhs.specular_) &&
                    rtc::Equal(lhs.shininess_, rhs.shininess_));
        }

        static Color GetDefaultColor() { return Color(1.0, 1.0, 1.0); };

        static double GetDefaultAmbient() { return 0.1; };

        static double GetDefaultDiffuse() { return 0.9; };

        static double GetDefaultSpecular() { return 0.9; };

        static double GetDefaultShininess() { return 200.0; };

    private:
        Color  color_;     ///< Color attribute for the Phong reflection model.
        double ambient_;   ///< Ambient attribute for the Phong reflection model.
        double diffuse_;   ///< Diffuse attribute for the Phong reflection model.
        double specular_;  ///< Specular attribute for the Phong reflection model.
        double shininess_; ///< Shininess attribute for the Phong reflection model.
    };
}
