cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui INTERFACE)
target_compile_features(voicemeeter_ui INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui INTERFACE wheel)
target_include_directories(voicemeeter_ui INTERFACE include)

add_library(Voicemeeter::UI ALIAS voicemeeter_ui)
