cmake_minimum_required(VERSION 3.25.1)

project(WUI::Graphics VERSION 0.9)

if(WUI_GRAPHICS STREQUAL D3D12)
	add_subdirectory(d3d12)
	add_library(WUI::Graphics ALIAS wui_graphics_d3d12)
elseif(WUI_GRAPHICS STREQUAL Vulkan)
	add_subdirectory(vulkan)
	add_library(WUI::Graphics ALIAS wui_graphics_vulkan)
else()
	message(FATAL_ERROR "Rendering backend is not supported")
endif()

include(GNUInstallDirs)
install(
	FILES ${PROJECT_SOURCE_DIR}/wuiGraphicsTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
