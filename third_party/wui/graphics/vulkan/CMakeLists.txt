cmake_minimum_required(VERSION 3.25.1)

project(WUI::Graphics::Vulkan VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(Vulkan)

add_library(_wui_graphics_vulkan INTERFACE)
target_compile_features(_wui_graphics_vulkan INTERFACE cxx_std_11)
target_link_libraries(_wui_graphics_vulkan
	INTERFACE
		::WSTD
		::Vulkan::API
		WUI::Layouts)
target_sources(_wui_graphics_vulkan
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/WUI/Atlas.hpp
		include/WUI/CachedFrame.hpp
		include/WUI/DirectFrame.hpp
		include/WUI/State.hpp
		include/WUI/Surface.hpp)

add_library(WUI::Graphics::Vulkan ALIAS _wui_graphics_vulkan)

set_property(
	TARGET wui_loader
	APPEND PROPERTY RESOURCE
		${PROJECT_SOURCE_DIR}/share/Vertex.hlsl
		${PROJECT_SOURCE_DIR}/share/MSDF.hlsl
		${PROJECT_SOURCE_DIR}/share/Blender.hlsl)

include(GNUInstallDirs)
install(
	TARGETS _wui_graphics_vulkan
	EXPORT _graphicsVulkanTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _graphicsVulkanTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::)
install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/graphicsVulkanTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
