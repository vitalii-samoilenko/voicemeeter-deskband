cmake_minimum_required(VERSION 3.25.1)

project(VBVMR VERSION 0.9 LANGUAGES C)

add_library(vbvmr INTERFACE)
target_compile_features(vbvmr INTERFACE c_std_99)
target_sources(vbvmr
	INTERFACE
		FILE_SET HEADERS
			BASE_DIRS include
			FILES include/VoicemeeterRemote.h)

add_library(VBVMR ALIAS vbvmr)

include(GNUInstallDirs)
install(
	TARGETS vbvmr
	EXPORT vbvmrTargets
	FILE_SET HEADERS
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT vbvmrTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VBVMR
	NAMESPACE ::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/VBVMRConfig.cmake.in
	${PROJECT_BINARY_DIR}/VBVMRConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VBVMR)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/VBVMRConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/VBVMRConfig.cmake
		${PROJECT_BINARY_DIR}/VBVMRConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VBVMR)
