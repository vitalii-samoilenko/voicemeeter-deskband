cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Loader.Windows VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_loader_windows INTERFACE)
if(LOADER STREQUAL Windows)
	add_library(Voicemeeter::Loader ALIAS voicemeeter_loader_windows)
endif()
target_compile_features(voicemeeter_loader_windows INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_loader_windows INTERFACE Windows)
target_include_directories(voicemeeter_loader_windows INTERFACE include)
