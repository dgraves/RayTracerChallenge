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

#include "ppm_writer.h"

#include "color.h"
#include "double_util.h"
#include "file_output_stream.h"

#include <cassert>

namespace rtc
{
    namespace PpmWriter
    {
        constexpr auto kMaxLineLength = 70u;
        const auto     kMagicNumber   = std::string{ "P3\n" };
        const auto     kMaxColorValue = std::string{ "255\n" };

        bool WriteFile(const std::string& filename, const Canvas& canvas)
        {
            auto stream = FileOutputStream{ filename };

            if (stream.IsValid())
            {
                return WriteStream(&stream, canvas);
            }

            return false;
        }

        bool WriteStream(OutputStream* stream, const Canvas& canvas)
        {
            auto success = false;

            if (stream != nullptr)
            {
                const auto width  = canvas.GetWidth();
                const auto height = canvas.GetHeight();

                success = WriteHeader(stream, width, height);
                success = success && WriteData(stream, canvas, width, height);
                success = success && stream->Write("\n", 1);
            }

            return success;
        }

        bool WriteHeader(OutputStream* stream, uint32_t width, uint32_t height)
        {
            auto success = false;

            if (stream != nullptr)
            {
                // Write "plain" PPM header.
                auto dim = std::to_string(width);
                dim += ' ';
                dim += std::to_string(height);
                dim += '\n';

                success = stream->Write(kMagicNumber.c_str(), kMagicNumber.length());
                success = success && stream->Write(dim.c_str(), dim.length());
                success = success && stream->Write(kMaxColorValue.c_str(), kMaxColorValue.length());
            }

            return success;
        }

        bool WriteData(OutputStream* stream, const Canvas& canvas, uint32_t width, uint32_t height)
        {
            auto success = false;

            if (stream != nullptr)
            {
                auto   line  = std::string{};
                size_t start = 0;

                success = true;

                for (uint32_t y = 0u; y < height; ++y)
                {
                    for (uint32_t x = 0u; x < width; ++x)
                    {
                        const auto pixel = canvas.PixelAt(x, y);

                        line += std::to_string(rtc::ToByte(pixel.GetR()));
                        line += ' ';
                        line += std::to_string(rtc::ToByte(pixel.GetG()));
                        line += ' ';
                        line += std::to_string(rtc::ToByte(pixel.GetB()));
                        line += ' ';

                        if ((line.length() - start) > kMaxLineLength)
                        {
                            // Find the index of the last space character closest to the line length limit.
                            size_t last = start + kMaxLineLength - 1;
                            while ((last > 0) && (line[last] != ' '))
                            {
                                --last;
                            }

                            // Convert the space charactrer to a new line character.
                            line[last] = '\n';

                            // Convert from index to count.
                            ++last;

                            success = success && stream->Write(&line[start], (last - start));

                            // New start position in string.
                            start = last;
                        }
                    }

                    auto length = line.length() - start;
                    if (length > 0)
                    {
                        // Convert the space charactrer at the end of the line to a new line character.
                        auto back = line.rbegin();
                        (*back) = '\n';

                        success = success && stream->Write(&line[start], length);
                        if (success)
                        {
                            line.clear();
                            start = 0;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            return success;
        }
    }
}

