cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Atlas.Msdfgen VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_atlas_msdfgen INTERFACE)

if(GENERATE_ATLAS)
	if(NOT DEFINED MSDFGEN_PATH)
		message(FATAL_ERROR "Msdfgen path is not specified")
	endif()

	add_executable(labels.generator)
	target_sources(labels.generator PRIVATE labels.cpp)
	target_compile_features(labels.generator PRIVATE cxx_std_17)
	foreach(N RANGE 70)
		list(APPEND NUMBERS_DESC ${PROJECT_BINARY_DIR}/${N}.desc)
	endforeach()
	add_custom_command(
		COMMAND labels.generator ${PROJECT_SOURCE_DIR}/shapes
		OUTPUT
			P.desc
			V.desc
			A1.desc
			A2.desc
			B1.desc
			B2.desc
			${NUMBERS_DESC}
		DEPENDS
			labels.generator
			P.desc
			V.desc
			A.desc
			B.desc
			0.desc
			1.desc
			2.desc
			3.desc
			4.desc
			5.desc
			6.desc
			7.desc
			8.desc
			9.desc
		VERBATIM)

	set(COMMON_PARAMS msdf -pxrange 4 -translate 1 1 -scale 1 -format binfloat)
	set(TEXT_DIMENSIONS 86 44)
	set(TEXT_RENDER_DIMENSIONS 1720 880)

	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_SOURCE_DIR}/shapes/knob_frame.desc
			-o knob_frame.bit
			-dimensions 50 50
			-testrender knob_frame.test.png 1000 1000
		DEPENDS shapes/knob_frame.desc
		OUTPUT knob_frame.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_SOURCE_DIR}/shapes/plug_frame.desc
			-o plug_frame.bit
			-dimensions 43 21
			-testrender plug_frame.test.png 860 420
		DEPENDS shapes/plug_frame.desc
		OUTPUT plug_frame.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_SOURCE_DIR}/shapes/knob_indicator.desc
			-o knob_indicator.bit
			-dimensions 24 24
			-testrender knob_indicator.test.png 480 480
		DEPENDS shapes/knob_indicator.desc
		OUTPUT knob_indicator.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_SOURCE_DIR}/shapes/vban_frame.desc
			-o vban_frame.bit
			-dimensions 80 46
			-testrender vban_frame.test.png 1600 920
		DEPENDS shapes/vban_frame.desc
		OUTPUT vban_frame.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_BINARY_DIR}/P.desc
			-o P.bit
			-dimensions ${TEXT_DIMENSIONS}
			-testrender P.test.png ${TEXT_RENDER_DIMENSIONS}
		DEPENDS ${PROJECT_BINARY_DIR}/P.desc
		OUTPUT P.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_BINARY_DIR}/V.desc
			-o V.bit
			-dimensions ${TEXT_DIMENSIONS}
			-testrender V.test.png ${TEXT_RENDER_DIMENSIONS}
		DEPENDS ${PROJECT_BINARY_DIR}/V.desc
		OUTPUT V.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_BINARY_DIR}/A1.desc
			-o A1.bit
			-dimensions ${TEXT_DIMENSIONS}
			-testrender A1.test.png ${TEXT_RENDER_DIMENSIONS}
		DEPENDS ${PROJECT_BINARY_DIR}/A1.desc
		OUTPUT A1.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_BINARY_DIR}/A2.desc
			-o A2.bit
			-dimensions ${TEXT_DIMENSIONS}
			-testrender A2.test.png ${TEXT_RENDER_DIMENSIONS}
		DEPENDS ${PROJECT_BINARY_DIR}/A2.desc
		OUTPUT A2.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_BINARY_DIR}/B1.desc
			-o B1.bit
			-dimensions ${TEXT_DIMENSIONS}
			-testrender B1.test.png ${TEXT_RENDER_DIMENSIONS}
		DEPENDS ${PROJECT_BINARY_DIR}/B1.desc
		OUTPUT B1.bit
		VERBATIM)
	add_custom_command(
		COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
			-shapedesc ${PROJECT_BINARY_DIR}/B2.desc
			-o B2.bit
			-dimensions ${TEXT_DIMENSIONS}
			-testrender B2.test.png ${TEXT_RENDER_DIMENSIONS}
		DEPENDS ${PROJECT_BINARY_DIR}/B2.desc
		OUTPUT B2.bit
		VERBATIM)
	foreach(N RANGE 70)
		list(APPEND NUMBERS_BIT ${PROJECT_BINARY_DIR}/${N}.bit)
		add_custom_command(
			COMMAND ${MSDFGEN_PATH} ${COMMON_PARAMS}
				-shapedesc ${PROJECT_BINARY_DIR}/${N}.desc
				-o ${N}.bit
				-dimensions ${TEXT_DIMENSIONS}
				-testrender ${N}.test.png ${TEXT_RENDER_DIMENSIONS}
				DEPENDS ${PROJECT_BINARY_DIR}/${N}.desc
				OUTPUT ${N}.bit
			VERBATIM)
	endforeach()
	add_executable(atlas.generator)
	target_sources(atlas.generator PRIVATE atlas.cpp)
	target_compile_features(atlas.generator PRIVATE cxx_std_17)
	add_custom_command(
		COMMAND atlas.generator
		OUTPUT Atlas.bit
		DEPENDS
			atlas.generator
			${PROJECT_BINARY_DIR}/knob_frame.bit
			${PROJECT_BINARY_DIR}/plug_frame.bit
			${PROJECT_BINARY_DIR}/knob_indicator.bit
			${PROJECT_BINARY_DIR}/vban_frame.bit
			${PROJECT_BINARY_DIR}/P.bit
			${PROJECT_BINARY_DIR}/V.bit
			${PROJECT_BINARY_DIR}/A1.bit
			${PROJECT_BINARY_DIR}/A2.bit
			${PROJECT_BINARY_DIR}/B1.bit
			${PROJECT_BINARY_DIR}/B2.bit
			${NUMBERS_BIT}
		VERBATIM)
	add_custom_target(run.atlas.generator DEPENDS ${PROJECT_BINARY_DIR}/Atlas.bit)
	add_dependencies(voicemeeter_ui_atlas_msdfgen run.atlas.generator)

	list(APPEND LOADER_FILES ${PROJECT_BINARY_DIR}/Atlas.bit)
else()
	list(APPEND LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Atlas.bit)
endif()

add_library(Voicemeeter::UI::Atlas ALIAS voicemeeter_ui_atlas_msdfgen)

list(APPEND LAYOUTS_BASE_DIRS ${PROJECT_SOURCE_DIR}/export)
list(APPEND LAYOUTS_FILES ${PROJECT_SOURCE_DIR}/export/Voicemeeter/UI/Layouts/Atlas.hpp)
set(LOADER_FILES ${LOADER_FILES} PARENT_SCOPE)
set(LAYOUTS_BASE_DIRS ${LAYOUTS_BASE_DIRS} PARENT_SCOPE)
set(LAYOUTS_FILES ${LAYOUTS_FILES} PARENT_SCOPE)
