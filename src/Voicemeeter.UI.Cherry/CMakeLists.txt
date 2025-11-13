cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Cherry VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_cherry INTERFACE)
add_library(Voicemeeter::UI::Cherry ALIAS voicemeeter_ui_cherry)
target_compile_features(voicemeeter_ui_cherry INTERFACE cxx_std_17)
target_include_directories(voicemeeter_ui_cherry INTERFACE include)
