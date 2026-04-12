cmake_minimum_required(VERSION 3.25.1)

project(WUI VERSION 0.9)

set(WUI_GRAPHICS "" CACHE STRING
	"Choose rendering backend: D3D12, Vulkan")
set(WUI_PALETTE "" CACHE STRING
	"Choose color space for interpolation: Oklab")
set(WUI_LOADER "" CACHE STRING
	"Choose resource manager: Windows")

add_subdirectory(layouts)
add_subdirectory(loader)
add_subdirectory(graphics)
add_subdirectory(palette)
add_subdirectory(scene)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/WUIConfig.cmake.in
	${PROJECT_BINARY_DIR}/cmake/WUIConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/cmake/WUIConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/cmake/WUIConfig.cmake
		${PROJECT_BINARY_DIR}/cmake/WUIConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
