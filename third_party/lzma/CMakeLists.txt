cmake_minimum_required(VERSION 3.25.1)

project(LZMA VERSION 0.9 LANGUAGES C)

if(NOT DEFINED LZMA_SDK_PATH)
	message(FATAL_ERROR "LZMA SDK path is not specified")
endif()

add_library(lzma STATIC)
target_compile_features(lzma PRIVATE c_std_99)
target_sources(lzma
	PRIVATE FILE_SET HEADERS
	BASE_DIRS ${LZMA_SDK_PATH}/C
	FILES
		${LZMA_SDK_PATH}/C/Compiler.h
		${LZMA_SDK_PATH}/C/Precomp.h
		${LZMA_SDK_PATH}/C/CpuArch.h
		${LZMA_SDK_PATH}/C/7zWindows.h
		${LZMA_SDK_PATH}/C/7zTypes.h
		${LZMA_SDK_PATH}/C/LzFind.h
		${LZMA_SDK_PATH}/C/LzHash.h
		${LZMA_SDK_PATH}/C/LzmaEnc.h
		${LZMA_SDK_PATH}/C/LzmaDec.h
		${LZMA_SDK_PATH}/C/Alloc.h)
target_sources(lzma
	PRIVATE
	${LZMA_SDK_PATH}/C/CpuArch.c
	${LZMA_SDK_PATH}/C/LzFind.c
	${LZMA_SDK_PATH}/C/LzmaEnc.c
	${LZMA_SDK_PATH}/C/LzmaDec.c
	${LZMA_SDK_PATH}/C/Alloc.c)

add_library(_lzma_enc INTERFACE)
target_compile_features(_lzma_enc INTERFACE c_std_99)
target_link_libraries(_lzma_enc INTERFACE lzma)
target_sources(_lzma_enc
	INTERFACE FILE_SET HEADERS
	BASE_DIRS ${LZMA_SDK_PATH}/C
	FILES
		${LZMA_SDK_PATH}/C/7zTypes.h
		${LZMA_SDK_PATH}/C/LzmaEnc.h
		${LZMA_SDK_PATH}/C/Alloc.h)

add_library(_lzma_dec INTERFACE)
target_compile_features(_lzma_dec INTERFACE c_std_99)
target_link_libraries(_lzma_dec INTERFACE lzma)
target_sources(_lzma_dec
	INTERFACE FILE_SET HEADERS
	BASE_DIRS ${LZMA_SDK_PATH}/C
	FILES
		${LZMA_SDK_PATH}/C/7zTypes.h
		${LZMA_SDK_PATH}/C/LzmaDec.h
		${LZMA_SDK_PATH}/C/Alloc.h)

add_library(LZMA::Enc ALIAS _lzma_enc)
add_library(LZMA::Dec ALIAS _lzma_dec)

include(GNUInstallDirs)
install(
	TARGETS
		lzma
		_lzma_enc
		_lzma_dec
	EXPORT lzmaTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
	EXPORT lzmaTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LZMA
	NAMESPACE ::)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/LZMAConfig.cmake.in
	${PROJECT_BINARY_DIR}/cmake/LZMAConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LZMA)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/cmake/LZMAConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/cmake/LZMAConfig.cmake
		${PROJECT_BINARY_DIR}/cmake/LZMAConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LZMA)
