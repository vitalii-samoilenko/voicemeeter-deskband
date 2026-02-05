cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Layouts VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_layouts INTERFACE)
add_library(Voicemeeter::UI::Layouts ALIAS voicemeeter_ui_layouts)
target_compile_features(voicemeeter_ui_layouts INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_layouts INTERFACE wheel)
target_sources(voicemeeter_ui_layouts INTERFACE
	FILE_SET export TYPE HEADERS
	BASE_DIRS
		${CMAKE_SOURCE_DIR}
		${CMAKE_BINARY_DIR}
	FILES ${LAYOUTS})
