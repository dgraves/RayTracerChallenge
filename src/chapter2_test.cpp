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
        WHEN("Members are checked")
        {
            THEN("c.red = -0.5, c.green = 0.4, c.blue = 1.7")
            {
                rtc::Color c(-0.5, 0.4, 1.7);

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
        WHEN("Colors are added")
        {
            THEN("c1 + c2 == color(1.6, 0.7, 1.0)")
            {
                rtc::Color c1(0.9, 0.6, 0.75);
                rtc::Color c2(0.7, 0.1, 0.25);

                REQUIRE(rtc::Color::Equal(rtc::Color::Add(c1, c2), rtc::Color(1.6, 0.7, 1.0)));

                c1.Add(c2);
                REQUIRE(c1.Equal(rtc::Color(1.6, 0.7, 1.0)));
            }
        }
    }
}

SCENARIO("Subtracting colors", "[color]")
{
    GIVEN("c1 <- color(0.9, 0.6, 0.75) and c2 <- color(0.7, 0.1, 0.25)")
    {
        WHEN("Colors are subtracted")
        {
            THEN("c1 - c2 == color(0.2, 0.5, 0.5)")
            {
                rtc::Color c1(0.9, 0.6, 0.75);
                rtc::Color c2(0.7, 0.1, 0.25);

                REQUIRE(rtc::Color::Equal(rtc::Color::Subtract(c1, c2), rtc::Color(0.2, 0.5, 0.5)));

                c1.Subtract(c2);
                REQUIRE(c1.Equal(rtc::Color(0.2, 0.5, 0.5)));
            }
        }
    }
}

SCENARIO("Multiplying a color by a scalar", "[color]")
{
    GIVEN("c <- color(0.2, 0.3, 0.4)")
    {
        WHEN("Color is multiplied by 2")
        {
            THEN("c * 2 == color(0.4, 0.6, 0.8)")
            {
                rtc::Color c(0.2, 0.3, 0.4);

                REQUIRE(rtc::Color::Equal(rtc::Color::Multiply(c, 2.0), rtc::Color(0.4, 0.6, 0.8)));

                c.Multiply(2.0);
                REQUIRE(c.Equal(rtc::Color(0.4, 0.6, 0.8)));
            }
        }
    }
}

SCENARIO("Multiplying colors", "[color]")
{
    GIVEN("c1 <- color(1, 0.2, 0.4) and c2 <- color(0.9, 1, 0.1)")
    {
        WHEN("Colors are multiplied")
        {
            rtc::Color c1(1.0, 0.2, 0.4);
            rtc::Color c2(0.9, 1.0, 0.1);

            THEN("c1 * c2 == color(0.9, 0.2, 0.04)")
            {
                REQUIRE(rtc::Color::Equal(rtc::Color::HadamardProduct(c1, c2), rtc::Color(0.9, 0.2, 0.04)));
            }
        }
    }
}

SCENARIO("Creating a canvas", "[canvas]")
{
    GIVEN("c <- canvas(10, 20)")
    {
        WHEN("When initialized")
        {
            THEN("c.width = 10 and c.height = 20 and every pixel of c is color(0, 0, 0)")
            {
                rtc::Canvas c(10, 20);

                REQUIRE(rtc::Equal(c.GetWidth(), 10.0));
                REQUIRE(rtc::Equal(c.GetHeight(), 20.0));

                for (uint32_t y = 0; y < 20; ++y)
                {
                    for (uint32_t x = 0; x < 10; ++x)
                    {
                        REQUIRE(rtc::Color::Equal(c.PixelAt(x, y), rtc::Color(0.0, 0.0, 0.0)));
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
        WHEN("c.write_pixel(2, 3, red)")
        {
            THEN("c.pixel_at(2, 3) = red")
            {
                rtc::Canvas c(10, 20);
                rtc::Color red(1.0, 0.0, 0.0);

                c.WritePixel(2, 3, red);

                REQUIRE(rtc::Color::Equal(c.PixelAt(2, 3), red));
            }
        }
    }
}

SCENARIO("Constructing the PPM header", "[canvas]")
{
    GIVEN("c <- canvas(5, 3)")
    {
        WHEN("ppm <- canvas_to_ppm(c)")
        {
            THEN("lines 1-3 of ppm are \"P3\n5 3\n255\n\"")
            {
                rtc::MemoryOutputStream stream;
                rtc::PpmWriter::WriteHeader(&stream, 5, 3);

                const std::string expected_header = "P3\n5 3\n255\n";
                std::string actual_header(stream.GetData(), std::next(stream.GetData(), stream.GetSize()));

                REQUIRE(expected_header == actual_header);
            }
        }
    }
}

SCENARIO("Constructing the PPM pixel data", "[canvas]")
{
    GIVEN("c <- canvas(5, 3) and c1 <- color(1.5, 0, 0) and c2 <- color(0, 0.5, 0) and c3 <- color(-0.5, 0, 1)")
    {
        WHEN("write_pixel(c, 0, 0, c1) and write_pixel(c, 2, 1, c2) and write_pixel(c, 4, 2, c3) and ppm <- canvas_to_ppm(c)")
        {
            THEN("lines 4-6 of ppm are \n\"255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                          "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                                          "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n\"")
            {
                rtc::Canvas c(5, 3);
                rtc::Color c1(1.5, 0.0, 0.0);
                rtc::Color c2(0.0, 0.5, 0.0);
                rtc::Color c3(-0.5, 0.0, 1.0);

                c.WritePixel(0, 0, c1);
                c.WritePixel(2, 1, c2);
                c.WritePixel(4, 2, c3);

                rtc::MemoryOutputStream stream;
                rtc::PpmWriter::WriteData(&stream, c, 5, 3);

                const std::string expected_data = "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                                  "0 0 0 0 0 0 0 127 0 0 0 0 0 0 0\n"
                                                  "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";
                std::string actual_data(stream.GetData(), std::next(stream.GetData(), stream.GetSize()));

                REQUIRE(expected_data == actual_data);
            }
        }
    }
}

SCENARIO("Splitting long lines of PPM files", "[canvas]")
{
    GIVEN("c <- canvas(10, 2)")
    {
        WHEN("every pixel of c is set to color(1, 0.8, 0.6)")
        {
            THEN("lines 4-7 of ppm are \n\"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                "153 255 204 153 255 204 153 255 204 153 255 204 153\n\"")
            {
                rtc::Canvas c(10, 2);
                c.Clear(rtc::Color(1.0, 0.8, 0.6));

                rtc::MemoryOutputStream stream;
                rtc::PpmWriter::WriteData(&stream, c, 10, 2);

                const std::string expected_data = "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                                                  "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                                                  "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                                                  "153 255 204 153 255 204 153 255 204 153 255 204 153\n";
                std::string actual_data(stream.GetData(), std::next(stream.GetData(), stream.GetSize()));

                REQUIRE(expected_data == actual_data);
            }
        }
    }
}

SCENARIO("PPM files are terminated by a newline", "[canvas]")
{
    GIVEN("c <- canvas(5, 3)")
    {
        WHEN("ppm <- canvas_to_ppm(c)")
        {
            THEN("Then the last character of ppm is a newline")
            {
                rtc::Canvas c(5, 3);

                rtc::MemoryOutputStream stream;
                rtc::PpmWriter::WriteStream(&stream, c);

                std::string actual_data(stream.GetData(), std::next(stream.GetData(), stream.GetSize()));

                REQUIRE(actual_data.back() == '\n');
            }
        }
    }
}
