cmake_minimum_required(VERSION 3.15.0)

project(RayTracerChallenge LANGUAGES CXX VERSION 0.0.1)

list(APPEND CMAKE_PREFIX_PATH "external/Catch2")

find_package(Catch2 REQUIRED)

add_executable(
    rtc
    src/double_util.h
    src/main.cpp
    src/point.h
    src/tuple.h
    src/vector.h)

target_compile_definitions(rtc PRIVATE -DNOMINMAX)

add_executable(tests src/main_test.cpp src/chapter1_test.cpp src/chapter2_test.cpp)
target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(tests)
