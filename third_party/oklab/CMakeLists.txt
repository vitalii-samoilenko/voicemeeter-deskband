cmake_minimum_required(VERSION 3.25.1)

project(Oklab VERSION 0.9 LANGUAGES CXX)

add_library(oklab INTERFACE)
target_compile_features(oklab INTERFACE cxx_std_11)
target_include_directories(oklab INTERFACE include)

add_library(Oklab ALIAS oklab)
