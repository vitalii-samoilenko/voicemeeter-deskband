cmake_minimum_required(VERSION 3.25.1)

project(WUI::Palette::Oklab VERSION 0.9 LANGUAGES CXX)

add_library(wui_palette_oklab INTERFACE)
target_compile_features(wui_palette_oklab INTERFACE cxx_std_11)
target_link_libraries(wui_palette_oklab
	INTERFACE
		wheel
		Oklab)
target_include_directories(wui_palette_oklab INTERFACE include)

add_library(WUI::Palette ALIAS wui_palette_oklab)
