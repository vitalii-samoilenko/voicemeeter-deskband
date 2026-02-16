cmake_minimum_required(VERSION 3.25.1)

project(wheel VERSION 0.9 LANGUAGES CXX)

option(EXTENDED_PRECISION "Use 64 bit num_t" ON)

add_library(wheel INTERFACE)
target_compile_features(wheel INTERFACE cxx_std_17)
target_include_directories(wheel INTERFACE include)
target_compile_definitions(wheel INTERFACE _USE_MATH_DEFINES)
target_compile_definitions(wheel INTERFACE NOMINMAX)
if(EXTENDED_PRECISION)
	target_compile_definitions(wheel INTERFACE WHEEL_EXTENDED_PRECISION)
endif()
