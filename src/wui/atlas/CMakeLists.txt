cmake_minimum_required(VERSION 3.25.1)

project(WUI::Atlas::Cherry VERSION 0.9 LANGUAGES CXX)

find_package(WUI)
find_package(msdfgen)

add_library(wui_atlas_cherry INTERFACE)

add_executable(wui_atlas_cherry_generator)
target_compile_features(wui_atlas_cherry_generator PRIVATE cxx_std_17)
target_sources(wui_atlas_cherry_generator PRIVATE main.cpp)

add_library(WUI::Atlas::Cherry ALIAS wui_atlas_cherry)
add_library(WUI::Atlas ALIAS wui_atlas_cherry)

add_custom_command(
	COMMAND wui_atlas_cherry_generator ${PROJECT_SOURCE_DIR}/shapes
	OUTPUT Atlas.bit
	DEPENDS
		bit.pre.transform
		shapes/0.desc
		shapes/1.desc
		shapes/2.desc
		shapes/3.desc
		shapes/4.desc
		shapes/5.desc
		shapes/6.desc
		shapes/7.desc
		shapes/8.desc
		shapes/9.desc
		shapes/A.desc
		shapes/B.desc
		shapes/knob_frame.desc
		shapes/knob_indicator.desc
		shapes/labels.info
		shapes/labels.kern
		shapes/P.desc
		shapes/plug_frame.desc
		shapes/vban_frame.desc
		shapes/V.desc
	VERBATIM)

set_property(
	TARGET wui_loader
	APPEND PROPERTY RESOURCE
		${PROJECT_BINARY_DIR}/Atlas.bit)

target_sources(wui_layouts
	INTERFACE FILE_SET HEADERS
	BASE_DIRS ${PROJECT_SOURCE_DIR}/include
	FILES ${PROJECT_SOURCE_DIR}/include/WUI/Layouts/Atlas.hpp)
