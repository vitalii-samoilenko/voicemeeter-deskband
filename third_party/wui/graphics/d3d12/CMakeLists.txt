cmake_minimum_required(VERSION 3.25.1)

project(WUI::Graphics::D3D12 VERSION 0.9 LANGUAGES CXX)

add_library(wui_graphics_d3d12 INTERFACE)
target_compile_features(wui_graphics_d3d12 INTERFACE cxx_std_11)
target_link_libraries(wui_graphics_d3d12
	INTERFACE
		estd
		wheel
		Windows
		WUI::Layouts)
target_include_directories(wui_graphics_d3d12 INTERFACE include)
target_link_options(wui_graphics_d3d12
	INTERFACE
		d3d12.lib
		d3dcompiler.lib
		dcomp.lib
		dxgi.lib)

list(APPEND WUI_LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Vertex.hlsl)
list(APPEND WUI_LOADER_FILES ${PROJECT_SOURCE_DIR}/export/MSDF.hlsl)
list(APPEND WUI_LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Blender.hlsl)
set(WUI_LOADER_FILES ${WUI_LOADER_FILES} PARENT_SCOPE)

add_library(WUI::Graphics ALIAS wui_graphics_d3d12)
