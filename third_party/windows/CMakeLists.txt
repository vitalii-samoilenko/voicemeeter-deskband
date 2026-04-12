cmake_minimum_required(VERSION 3.25.1)

project(Windows VERSION 0.9 LANGUAGES CXX)

add_library(_windows_api INTERFACE)
target_compile_features(_windows_api INTERFACE cxx_std_11)
target_sources(_windows_api
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Windows/Error.hpp
		include/Windows/API.hpp)

add_library(_windows_com INTERFACE)
target_compile_features(_windows_com INTERFACE cxx_std_11)
target_sources(_com
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Windows/Error.hpp
		include/Windows/COM.hpp)

add_library(_windows_d3d12 INTERFACE)
target_compile_features(_windows_d3d12 INTERFACE cxx_std_11)
target_link_libraries(_windows_d3d12
	INTERFACE
		dxgi
		d3d12
		d3dcompiler)

add_library(Windows::API ALIAS _windows_api)
add_library(Windows::COM ALIAS _windows_com)
add_library(Windows::D3D12 ALIAS _windows_d3d12)

include(GNUInstallDirs)
install(
	TARGETS
		_windows_api
		_windows_com
		_windows_d3d12
	EXPORT windowsTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT windowsTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windows
	NAMESPACE ::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/WindowsConfig.cmake.in
	${PROJECT_BINARY_DIR}/cmake/WindowsConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windows)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/cmake/WindowsConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/cmake/WindowsConfig.cmake
		${PROJECT_BINARY_DIR}/cmake/WindowsConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windows)
