cmake_minimum_required(VERSION 3.25.1)

project(WSTD VERSION 0.9 LANGUAGES CXX)

set(WSTD_EXTENDED_PRECISION ON CACHE BOOL
	"Use 64 bit num_t")

add_library(wstd INTERFACE)
target_compile_features(wstd INTERFACE cxx_std_11)
target_compile_definitions(wstd INTERFACE _USE_MATH_DEFINES)
if(WSTD_EXTENDED_PRECISION)
	target_compile_definitions(wstd INTERFACE WSTD_EXTENDED_PRECISION)
endif()
target_sources(wstd
	INTERFACE
		FILE_SET HEADERS
			BASE_DIRS include
			FILES
				include/math.hpp
				include/memory.hpp)

add_library(WSTD ALIAS wstd)

include(GNUInstallDirs)
install(
	TARGETS wstd
	EXPORT wstdTargets
	FILE_SET HEADERS
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT wstdTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WSTD
	NAMESPACE ::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/WSTDConfig.cmake.in
	${PROJECT_BINARY_DIR}/WSTDConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WSTD)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/WSTDConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/WSTDConfig.cmake
		${PROJECT_BINARY_DIR}/WSTDConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WSTD)
