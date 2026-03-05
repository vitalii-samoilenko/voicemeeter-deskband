cmake_minimum_required(VERSION 3.25.1)

project(Windows VERSION 0.9 LANGUAGES CXX)

add_library(windows INTERFACE)
target_compile_features(windows INTERFACE cxx_std_11)
target_sources(windows
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Windows/Error.hpp
		include/Windows/API.hpp
		include/Windows/COM.hpp)

add_library(Windows ALIAS windows)

include(GNUInstallDirs)
install(
	TARGETS windows
	EXPORT windowsTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT windowsTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windows
	NAMESPACE ::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/WindowsConfig.cmake.in
	${PROJECT_BINARY_DIR}/WindowsConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windows)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/WindowsConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/WindowsConfig.cmake
		${PROJECT_BINARY_DIR}/WindowsConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windows)
