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

#include "catch2/catch.hpp"

#include "canvas.h"
#include "color.h"
#include "double_util.h"
#include "memory_output_stream.h"
#include "ppm_writer.h"

SCENARIO("Colors are (red, green, blue) tuples", "[color]")
{
    GIVEN("c <- color(-0.5, 0.4, 1.7)")
    {
        const auto c = rtc::Color{ -0.5, 0.4, 1.7 };

        WHEN("Members are checked")
        {
            THEN("c.red = -0.5, c.green = 0.4, c.blue = 1.7")
            {
                REQUIRE(rtc::Equal(c.GetR(), -0.5));
                REQUIRE(rtc::Equal(c.GetG(), 0.4));
                REQUIRE(rtc::Equal(c.GetB(), 1.7));
            }
        }
    }
}

SCENARIO("Adding colors", "[color]")
{
    GIVEN("c1 <- color(0.9, 0.6, 0.75) and c2 <- color(0.7, 0.1, 0.25)")
    {
        auto       c1 = rtc::Color{ 0.9, 0.6, 0.75 };
        const auto c2 = rtc::Color{ 0.7, 0.1, 0.25 };

        WHEN("Colors are added")
        {
            THEN("c1 + c2 == color(1.6, 0.7, 1.0)")
            {
                REQUIRE(rtc::Color::Equal(rtc::Color::Add(c1, c2), rtc::Color{ 1.6, 0.7, 1.0 }));

                c1.Add(c2);
                REQUIRE(c1.Equal(rtc::Color{ 1.6, 0.7, 1.0 }));
            }
        }
    }
}

SCENARIO("Subtracting colors", "[color]")
{
    GIVEN("c1 <- color(0.9, 0.6, 0.75) and c2 <- color(0.7, 0.1, 0.25)")
    {
        auto       c1 = rtc::Color{ 0.9, 0.6, 0.75 };
        const auto c2 = rtc::Color{ 0.7, 0.1, 0.25 };

        WHEN("Colors are subtracted")
        {
            THEN("c1 - c2 == color(0.2, 0.5, 0.5)")
            {
                REQUIRE(rtc::Color::Equal(rtc::Color::Subtract(c1, c2), rtc::Color{ 0.2, 0.5, 0.5 }));

                c1.Subtract(c2);
                REQUIRE(c1.Equal(rtc::Color{ 0.2, 0.5, 0.5 }));
            }
        }
    }
}

SCENARIO("Multiplying a color by a scalar", "[color]")
{
    GIVEN("c <- color(0.2, 0.3, 0.4)")
    {
        auto c = rtc::Color{ 0.2, 0.3, 0.4 };

        WHEN("Color is multiplied by 2")
        {
            THEN("c * 2 == color(0.4, 0.6, 0.8)")
            {
                REQUIRE(rtc::Color::Equal(rtc::Color::Multiply(c, 2.0), rtc::Color{ 0.4, 0.6, 0.8 }));

                c.Multiply(2.0);
                REQUIRE(c.Equal(rtc::Color{ 0.4, 0.6, 0.8 }));
            }
        }
    }
}

SCENARIO("Multiplying colors", "[color]")
{
    GIVEN("c1 <- color(1, 0.2, 0.4) and c2 <- color(0.9, 1, 0.1)")
    {
        const auto c1 = rtc::Color{ 1.0, 0.2, 0.4 };
        const auto c2 = rtc::Color{ 0.9, 1.0, 0.1 };

        WHEN("Colors are multiplied")
        {
            THEN("c1 * c2 == color(0.9, 0.2, 0.04)")
            {
                REQUIRE(rtc::Color::Equal(rtc::Color::HadamardProduct(c1, c2), rtc::Color{ 0.9, 0.2, 0.04 }));
            }
        }
    }
}

SCENARIO("Creating a canvas", "[canvas]")
{
    GIVEN("c <- canvas(10, 20)")
    {
        const auto c = rtc::Canvas{ 10u, 20u };

        WHEN("When initialized")
        {
            THEN("c.width = 10 and c.height = 20 and every pixel of c is color(0, 0, 0)")
            {
                REQUIRE(rtc::Equal(c.GetWidth(), 10.0));
                REQUIRE(rtc::Equal(c.GetHeight(), 20.0));

                const auto expected = rtc::Color{ 0.0, 0.0, 0.0 };
                for (uint32_t y = 0u; y < 20u; ++y)
                {
                    for (uint32_t x = 0u; x < 10u; ++x)
                    {
                        REQUIRE(rtc::Color::Equal(c.PixelAt(x, y), expected));
                    }
                }
            }
        }
    }
}

SCENARIO("Writing pixels to a canvas", "[canvas]")
{
    GIVEN("c <- canvas(10, 20) and red <- color(1, 0, 0)")
    {
        auto       c   = rtc::Canvas{ 10u, 20u };
        const auto red = rtc::Color{ 1.0, 0.0, 0.0 };

        WHEN("c.write_pixel(2, 3, red)")
        {
            c.WritePixel(2u, 3u, red);

            THEN("c.pixel_at(2, 3) = red")
            {
                REQUIRE(rtc::Color::Equal(c.PixelAt(2u, 3u), red));
            }
        }
    }
}

SCENARIO("Constructing the PPM header", "[canvas]")
{
    GIVEN("c <- canvas(5, 3)")
    {
        const auto c = rtc::Canvas{ 5u, 3u };

        WHEN("ppm <- canvas_to_ppm(c)")
        {
            auto stream = rtc::MemoryOutputStream{};
            rtc::PpmWriter::WriteStream(&stream, c);

            const auto ppm      = stream.GetData();
            const auto ppm_size = stream.GetSize();

            THEN("lines 1-3 of ppm are \"P3\n5 3\n255\n\"")
            {
                // Find the position after the first three lines.
                auto pos        = 0u;
                auto line_count = 0u;
                while ((line_count < 3u) && (pos < ppm_size))
                {
                    if (ppm[pos++] == '\n')
                    {
                        ++line_count;
                    }
                }

                REQUIRE(std::string(ppm, std::next(ppm, pos)) == std::string{ "P3\n5 3\n255\n" });
            }
        }
    }
}

SCENARIO("Constructing the PPM pixel data", "[canvas]")
{
    GIVEN("c <- canvas(5, 3) and c1 <- color(1.5, 0, 0) and c2 <- color(0, 0.5, 0) and c3 <- color(-0.5, 0, 1)")
    {
        auto       c  = rtc::Canvas{ 5u, 3u };
        const auto c1 = rtc::Color{ 1.5, 0.0, 0.0 };
        const auto c2 = rtc::Color{ 0.0, 0.5, 0.0 };
        const auto c3 = rtc::Color{ -0.5, 0.0, 1.0 };

        WHEN("write_pixel(c, 0, 0, c1) and write_pixel(c, 2, 1, c2) and write_pixel(c, 4, 2, c3) and ppm <- canvas_to_ppm(c)")
        {
            c.WritePixel(0u, 0u, c1);
            c.WritePixel(2u, 1u, c2);
            c.WritePixel(4u, 2u, c3);

            auto stream = rtc::MemoryOutputStream{};
            rtc::PpmWriter::WriteData(&stream, c, 5u, 3u);

            const auto ppm      = stream.GetData();
            const auto ppm_size = stream.GetSize();

            THEN("lines 4-6 of ppm are \n\"255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                          "0 0 0 0 0 0 0 127 0 0 0 0 0 0 0\n"
                                          "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n\"")
            {
                const auto expected_data = std::string{ "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                                        "0 0 0 0 0 0 0 127 0 0 0 0 0 0 0\n"
                                                        "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n" };

                REQUIRE(std::string(ppm, std::next(ppm, ppm_size)) == expected_data);
            }
        }
    }
}

SCENARIO("Splitting long lines of PPM files", "[canvas]")
{
    GIVEN("c <- canvas(10, 2)")
    {
        auto c = rtc::Canvas{ 10u, 2u };

        WHEN("every pixel of c is set to color(1, 0.8, 0.6)")
        {
            c.Clear(rtc::Color{ 1.0, 0.8, 0.6 });

            auto stream = rtc::MemoryOutputStream{};
            rtc::PpmWriter::WriteData(&stream, c, 10u, 2u);

            const auto ppm      = stream.GetData();
            const auto ppm_size = stream.GetSize();

            THEN("lines 4-7 of ppm are \n\""
                "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                "153 255 204 153 255 204 153 255 204 153 255 204 153\n\"")
            {
                const auto expected_data = std::string{ "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                                                        "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                                                        "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                                                        "153 255 204 153 255 204 153 255 204 153 255 204 153\n" };

                REQUIRE(std::string(ppm, std::next(ppm, ppm_size)) == expected_data);
            }
        }
    }
}

SCENARIO("PPM files are terminated by a newline", "[canvas]")
{
    GIVEN("c <- canvas(5, 3)")
    {
        const auto c = rtc::Canvas{ 5u, 3u };

        WHEN("ppm <- canvas_to_ppm(c)")
        {
            auto stream = rtc::MemoryOutputStream{};
            rtc::PpmWriter::WriteStream(&stream, c);

            const auto ppm      = stream.GetData();
            const auto ppm_size = stream.GetSize();

            THEN("Then the last character of ppm is a newline")
            {
                REQUIRE(ppm_size > 0);
                REQUIRE(ppm[ppm_size - 1] == '\n');
            }
        }
    }
}
