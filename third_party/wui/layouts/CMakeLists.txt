cmake_minimum_required(VERSION 3.25.1)

project(WUI::Layouts VERSION 0.9 LANGUAGES CXX)

add_library(wui_layouts INTERFACE)
target_compile_features(wui_layouts INTERFACE cxx_std_11)
target_link_libraries(wui_layouts INTERFACE wheel)
target_sources(wui_layouts INTERFACE
	FILE_SET export TYPE HEADERS
	BASE_DIRS ${LAYOUTS_BASE_DIRS}
	FILES ${LAYOUTS_FILES})

add_library(WUI::Layouts ALIAS wui_layouts)
