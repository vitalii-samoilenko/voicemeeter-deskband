cmake_minimum_required(VERSION 3.25.1)

project(estd VERSION 0.9 LANGUAGES CXX)

add_library(estd INTERFACE)
target_compile_features(estd INTERFACE cxx_std_17)
target_include_directories(estd INTERFACE include)
