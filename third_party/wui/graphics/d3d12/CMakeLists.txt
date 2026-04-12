cmake_minimum_required(VERSION 3.25.1)

project(WUI::Graphics::D3D12 VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(Windows
	REQUIRED COMPONENTS
		API
		COM
		D3D12)

add_library(_wui_graphics_d3d12 INTERFACE)
target_compile_features(_wui_graphics_d3d12 INTERFACE cxx_std_11)
target_link_libraries(_wui_graphics_d3d12
	INTERFACE
		::WSTD
		::Windows::API
		::Windows::COM
		::Windows::D3D12
		Layouts)
target_sources(_wui_graphics_d3d12
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/WUI/Graphics/Atlas.hpp
		include/WUI/Graphics/CachedFrame.hpp
		include/WUI/Graphics/DirectFrame.hpp
		include/WUI/Graphics/State.hpp
		include/WUI/Graphics/Surface.hpp)

add_library(Graphics::D3D12 ALIAS _wui_graphics_d3d12)

set_property(
	TARGET wui_loader
	APPEND PROPERTY RESOURCE
		${PROJECT_SOURCE_DIR}/share/Vertex.hlsl
		${PROJECT_SOURCE_DIR}/share/MSDF.hlsl
		${PROJECT_SOURCE_DIR}/share/Blender.hlsl)

include(GNUInstallDirs)
install(
	TARGETS _wui_graphics_d3d12
	EXPORT _graphicsD3d12Targets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _graphicsD3d12Targets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::WUI)
install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/graphicsD3d12Targets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
