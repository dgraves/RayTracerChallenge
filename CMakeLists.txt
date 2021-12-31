cmake_minimum_required(VERSION 3.15.0)

project(RayTracerChallenge LANGUAGES CXX VERSION 0.0.1)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 20)
endif()

list(APPEND CMAKE_PREFIX_PATH "external/Catch2")

find_package(Catch2 REQUIRED)

add_library(rtc_lib STATIC
    src/camera.h
    src/camera.cpp
    src/canvas.h
    src/canvas.cpp
    src/color.h
    src/computations.h
    src/double_util.h
    src/file_output_stream.h
    src/file_output_stream.cpp
    src/intersection.h
    src/intersections.h
    src/intersections.cpp
    src/output_stream.h
    src/material.h
    src/matrix.h
    src/matrix22.h
    src/matrix33.h
    src/matrix44.h
    src/memory_output_stream.h
    src/phong.h
    src/phong.cpp
    src/plane.h
    src/plane.cpp
    src/point.h
    src/point_light.h
    src/ppm_writer.h
    src/ppm_writer.cpp
    src/shape.h
    src/sphere.h
    src/sphere.cpp
    src/ray.h
    src/tuple.h
    src/vector.h
    src/world.h
    src/world.cpp)
target_compile_definitions(rtc_lib PRIVATE -DNOMINMAX)

add_executable(rtc src/main.cpp)
target_link_libraries(rtc PRIVATE rtc_lib)

add_executable(tests
    src/main_test.cpp
    src/chapter1_test.cpp
    src/chapter2_test.cpp
    src/chapter3_test.cpp
    src/chapter4_test.cpp
    src/chapter5_test.cpp
    src/chapter6_test.cpp
    src/chapter7_test.cpp
    src/chapter8_test.cpp
    src/chapter9_test.cpp)
target_link_libraries(tests PRIVATE rtc_lib Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)
