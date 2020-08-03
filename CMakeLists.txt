cmake_minimum_required(VERSION 3.15.0)

project(RayTracerChallenge LANGUAGES CXX VERSION 0.0.1)

list(APPEND CMAKE_PREFIX_PATH "external/Catch2")

find_package(Catch2 REQUIRED)

add_library(rtc_lib STATIC
    src/canvas.h
    src/canvas.cpp
    src/color.h
    src/double_util.h
    src/point.h
    src/tuple.h
    src/vector.h)
target_compile_definitions(rtc_lib PRIVATE -DNOMINMAX)

add_executable(rtc src/main.cpp)
target_link_libraries(rtc rtc_lib)

add_executable(tests src/main_test.cpp src/chapter1_test.cpp src/chapter2_test.cpp)
target_link_libraries(tests rtc_lib Catch2::Catch2)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(tests)
