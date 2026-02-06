cmake_minimum_required(VERSION 3.25.1)

project(wheel VERSION 0.9 LANGUAGES CXX)

add_library(wheel INTERFACE)
target_compile_features(wheel INTERFACE cxx_std_17)
target_include_directories(wheel INTERFACE include)
target_compile_definitions(wheel INTERFACE _USE_MATH_DEFINES)
target_compile_definitions(wheel INTERFACE NOMINMAX)
