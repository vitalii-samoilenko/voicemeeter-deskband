cmake_minimum_required(VERSION 3.25.1)

project(wheel VERSION 0.9 LANGUAGES CXX)

set(WHEEL_EXTENDED_PRECISION ON CACHE BOOL
	"Use 64 bit num_t")

add_library(wheel INTERFACE)
target_compile_features(wheel INTERFACE cxx_std_11)
target_include_directories(wheel INTERFACE include)
target_compile_definitions(wheel INTERFACE _USE_MATH_DEFINES)
if(WHEEL_EXTENDED_PRECISION)
	target_compile_definitions(wheel INTERFACE WHEEL_EXTENDED_PRECISION)
endif()
