cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Atlas.Msdfgen VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_atlas_msdfgen INTERFACE)

if(NOT DEFINED MSDFGEN_PATH)
	message(FATAL_ERROR "Msdfgen path is not specified")
endif()

add_custom_command(
	COMMAND ${MSDFGEN_PATH} msdf
		-shapedesc ${PROJECT_SOURCE_DIR}/shapes/knob_frame.desc
		-o knob_frame.bit
		-dimensions 50 50
		-pxrange 4
		-translate 1 1
		-scale 1
		-format binfloat
		-testrender knob_frame.test.png 1000 1000
	DEPENDS shapes/knob_frame.desc
	OUTPUT knob_frame.bit
	VERBATIM)
add_custom_command(
	COMMAND ${MSDFGEN_PATH} msdf
		-shapedesc ${PROJECT_SOURCE_DIR}/shapes/plug_frame.desc
		-o plug_frame.bit
		-dimensions 43 21
		-pxrange 4
		-scale 1
		-translate 1 1
		-format binfloat
		-testrender plug_frame.test.png 860 420
	DEPENDS shapes/plug_frame.desc
	OUTPUT plug_frame.bit
	VERBATIM)
add_custom_command(
	COMMAND ${MSDFGEN_PATH} msdf
		-shapedesc ${PROJECT_SOURCE_DIR}/shapes/knob_indicator.desc
		-o knob_indicator.bit
		-dimensions 24 24
		-pxrange 4
		-scale 1
		-translate 1 1
		-format binfloat
		-testrender knob_indicator.test.png 480 480
	DEPENDS shapes/knob_indicator.desc
	OUTPUT knob_indicator.bit
	VERBATIM)
add_custom_command(
	COMMAND ${MSDFGEN_PATH} msdf
		-shapedesc ${PROJECT_SOURCE_DIR}/shapes/vban_frame.desc
		-o vban_frame.bit
		-dimensions 80 46
		-pxrange 4
		-scale 1
		-translate 1 1
		-format binfloat
		-testrender vban_frame.test.png 1600 920
	DEPENDS shapes/vban_frame.desc
	OUTPUT vban_frame.bit
	VERBATIM)
add_executable(atlas.generator)
target_sources(atlas.generator PRIVATE atlas.cpp)
target_compile_features(atlas.generator PRIVATE cxx_std_17)
add_custom_command(
	COMMAND atlas.generator
	OUTPUT Atlas.bit
	DEPENDS
		atlas.generator
		knob_frame.bit
		plug_frame.bit
		knob_indicator.bit
		vban_frame.bit
	VERBATIM)
add_custom_target(run.atlas.generator DEPENDS Atlas.bit)
add_dependencies(voicemeeter_ui_atlas_msdfgen run.atlas.generator)

add_library(Voicemeeter::UI::Atlas ALIAS voicemeeter_ui_atlas_msdfgen)

list(APPEND LOADER_FILES ${PROJECT_BINARY_DIR}/Atlas.bit)
list(APPEND LAYOUTS_BASE_DIRS ${PROJECT_SOURCE_DIR}/export)
list(APPEND LAYOUTS_FILES ${PROJECT_SOURCE_DIR}/export/Voicemeeter/UI/Layouts/Atlas.hpp)
set(LOADER_FILES ${LOADER_FILES} PARENT_SCOPE)
set(LAYOUTS_BASE_DIRS ${LAYOUTS_BASE_DIRS} PARENT_SCOPE)
set(LAYOUTS_FILES ${LAYOUTS_FILES} PARENT_SCOPE)
