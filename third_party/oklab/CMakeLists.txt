cmake_minimum_required(VERSION 3.25.1)

project(Oklab VERSION 0.9 LANGUAGES CXX)

add_library(_oklab INTERFACE)
target_compile_features(_oklab INTERFACE cxx_std_11)
target_sources(_oklab
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/ok_color.h)

add_library(Oklab ALIAS _oklab)

include(GNUInstallDirs)
install(
	TARGETS _oklab
	EXPORT oklabTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT oklabTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Oklab
	NAMESPACE ::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/OklabConfig.cmake.in
	${PROJECT_BINARY_DIR}/cmake/OklabConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Oklab)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/cmake/OklabConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/cmake/OklabConfig.cmake
		${PROJECT_BINARY_DIR}/cmake/OklabConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Oklab)
