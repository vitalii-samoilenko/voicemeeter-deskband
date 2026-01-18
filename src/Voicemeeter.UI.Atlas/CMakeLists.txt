cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Atlas VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_atlas INTERFACE)
add_library(Voicemeeter::UI::Atlas ALIAS voicemeeter_ui_atlas)
target_compile_features(voicemeeter_ui_atlas INTERFACE cxx_std_17)
target_include_directories(voicemeeter_ui_atlas INTERFACE include)
