cmake_minimum_required(VERSION 3.25.1)

project(WUI::Graphics::Vulkan VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)

add_library(wui_graphics_vulkan INTERFACE)
target_compile_features(wui_graphics_vulkan INTERFACE cxx_std_11)
target_link_libraries(wui_graphics_vulkan
	INTERFACE
		::WSTD
		WUI::Layouts)
target_link_libraries(wui_graphics_vulkan INTERFACE vulkan)
target_sources(wui_graphics_vulkan
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/WUI/Atlas.hpp
		include/WUI/CachedFrame.hpp
		include/WUI/DirectFrame.hpp
		include/WUI/State.hpp
		include/WUI/Surface.hpp)

add_library(WUI::Graphics::Vulkan ALIAS wui_graphics_vulkan)

set_property(
	TARGET wui_loader
	APPEND PROPERTY RESOURCE
		${PROJECT_SOURCE_DIR}/share/Vertex.hlsl
		${PROJECT_SOURCE_DIR}/share/MSDF.hlsl
		${PROJECT_SOURCE_DIR}/share/Blender.hlsl)

include(GNUInstallDirs)
install(
	TARGETS wui_graphics_vulkan
	EXPORT _wuiGraphicsVulkanTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _wuiGraphicsVulkanTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::)
install(
	FILES ${PROJECT_SOURCE_DIR}/wuiGraphicsVulkanTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
