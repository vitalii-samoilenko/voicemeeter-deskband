cmake_minimum_required(VERSION 3.25.1)

project(WUI::Atlas::Cherry VERSION 0.9 LANGUAGES CXX)

add_library(wui_atlas_cherry INTERFACE)

find_package(WUI)
find_package(msdfgen)

add_executable(bit.pre.transform)
target_compile_features(bit.pre.transform PRIVATE cxx_std_17)
target_sources(bit.pre.transform PRIVATE bit.pre.transform.cpp)

add_executable(bit.combine)
target_compile_features(bit.combine PRIVATE cxx_std_17)
target_link_libraries(bit.combine PRIVATE msdfgen::msdfgen)
target_sources(bit.combine PRIVATE bit.combine.cpp)

add_library(WUI::Atlas::Cherry ALIAS wui_atlas_cherry)
add_library(WUI::Atlas ALIAS wui_atlas_cherry)

foreach(N RANGE 70)
	list(APPEND WUI_ATLAS_NUMBERS_DESC ${PROJECT_BINARY_DIR}/${N}.desc)
endforeach()
add_custom_command(
	COMMAND bit.pre.transform ${PROJECT_SOURCE_DIR}/shapes
	OUTPUT
		${WUI_ATLAS_NUMBERS_DESC}
		A1.desc
		A2.desc
		B1.desc
		B2.desc
		P.desc
		V.desc
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
		shapes/labels.info
		shapes/labels.kern
		shapes/P.desc
		shapes/V.desc
	VERBATIM)
add_custom_command(
	COMMAND bit.combine ${PROJECT_SOURCE_DIR}/shapes
	OUTPUT Atlas.bit
	DEPENDS
		bit.combine
		knob_frame.desc
		knob_indicator.desc
		plug_frame.desc
		vban_frame.desc
		${WUI_ATLAS_NUMBERS_DESC}
		${PROJECT_BINARY_DIR}/A1.desc
		${PROJECT_BINARY_DIR}/A2.desc
		${PROJECT_BINARY_DIR}/B1.desc
		${PROJECT_BINARY_DIR}/B2.desc
		${PROJECT_BINARY_DIR}/P.desc
		${PROJECT_BINARY_DIR}/V.desc
	VERBATIM)

set_property(
	TARGET wui_loader
	APPEND PROPERTY RESOURCE
		${PROJECT_BINARY_DIR}/Atlas.bit)

target_sources(wui_layouts
	INTERFACE FILE_SET HEADERS
	BASE_DIRS ${PROJECT_SOURCE_DIR}/include
	FILES ${PROJECT_SOURCE_DIR}/include/WUI/Layouts/Atlas.hpp)
