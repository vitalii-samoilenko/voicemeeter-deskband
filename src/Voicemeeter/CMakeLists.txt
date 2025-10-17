cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter INTERFACE)
add_library(Voicemeeter ALIAS voicemeeter)
target_compile_features(voicemeeter INTERFACE cxx_std_17)
target_include_directories(voicemeeter INTERFACE include)
