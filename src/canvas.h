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

#include <cinttypes>
#include <vector>

namespace rtc
{
    class Canvas
    {
    public:
        Canvas(uint32_t width, uint32_t height);

        uint32_t GetWidth() const { return width_; }

        uint32_t GetHeight() const { return height_; }

        void WritePixel(uint32_t x, uint32_t y, const Color& color) { pixels_[y][x] = color; }

        void WritePixel(uint32_t x, uint32_t y, Color&& color) { pixels_[y][x] = std::move(color); }

        const Color& PixelAt(uint32_t x, uint32_t y) const { return pixels_[y][x]; }

        void Clear(const Color& color);

    private:
        using PixelRow = std::vector<Color>;

    private:
        uint32_t              width_;
        uint32_t              height_;
        std::vector<PixelRow> pixels_;
    };
}
