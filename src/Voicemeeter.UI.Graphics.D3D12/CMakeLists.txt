cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Graphics.D3D12 VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_graphics_d3d12 INTERFACE)
target_compile_features(voicemeeter_ui_graphics_d3d12 INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_graphics_d3d12 INTERFACE
	wheel
	Windows
	Voicemeeter::UI::Layouts)
target_include_directories(voicemeeter_ui_graphics_d3d12 INTERFACE include)
target_link_options(voicemeeter_ui_graphics_d3d12 INTERFACE
	d3d12.lib
	d3dcompiler.lib
	dcomp.lib
	dxgi.lib)

add_library(Voicemeeter::UI::Graphics ALIAS voicemeeter_ui_graphics_d3d12)

list(APPEND LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Vertex.hlsl)
list(APPEND LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Pixel.hlsl)
set(LOADER_FILES ${LOADER_FILES} PARENT_SCOPE)
