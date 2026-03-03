cmake_minimum_required(VERSION 3.25.1)

project(WUI::Scene VERSION 0.9 LANGUAGES CXX)

add_library(wui_scene INTERFACE)
target_compile_features(wui_scene INTERFACE cxx_std_17)
target_link_libraries(wui_scene INTERFACE wheel)
target_include_directories(wui_scene INTERFACE include)

add_library(WUI::Scene ALIAS wui_scene)
