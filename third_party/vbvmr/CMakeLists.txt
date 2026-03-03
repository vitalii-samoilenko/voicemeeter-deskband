cmake_minimum_required(VERSION 3.25.1)

project(VBVMR VERSION 0.9 LANGUAGES C)

add_library(vbvmr INTERFACE)
target_compile_features(vbvmr INTERFACE c_std_99)
target_include_directories(vbvmr INTERFACE include)

add_library(VBVMR ALIAS vbvmr)
