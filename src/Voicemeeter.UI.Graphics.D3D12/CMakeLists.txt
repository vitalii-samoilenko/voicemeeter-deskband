cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Graphics.D3D12 VERSION 0.9 LANGUAGES CXX)

list(APPEND LOADER_ITEMS ${PROJECT_SOURCE_DIR}/export/Vertex.hlsl)
list(APPEND LOADER_ITEMS ${PROJECT_SOURCE_DIR}/export/Pixel.hlsl)

add_library(voicemeeter_ui_graphics_d3d12 INTERFACE)
if(GRAPHICS STREQUAL D3D12)
	set(LOADER_ITEMS ${LOADER_ITEMS} PARENT_SCOPE)
	add_library(Voicemeeter::UI::Graphics ALIAS voicemeeter_ui_graphics_d3d12)
endif()
target_compile_features(voicemeeter_ui_graphics_d3d12 INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_graphics_d3d12 INTERFACE
	wheel
	Windows
	Voicemeeter::UI::Layouts)
target_include_directories(voicemeeter_ui_graphics_d3d12 INTERFACE include)
