cmake_minimum_required(VERSION 3.25.1)

project(Windows VERSION 0.9 LANGUAGES CXX)

add_library(windows INTERFACE)
target_compile_features(windows INTERFACE cxx_std_17)
target_link_libraries(windows INTERFACE
	estd
	wheel)
target_include_directories(windows INTERFACE include)

add_library(Windows ALIAS windows)
