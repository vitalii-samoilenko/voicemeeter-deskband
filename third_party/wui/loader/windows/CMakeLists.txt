cmake_minimum_required(VERSION 3.25.1)

project(WUI::Loader::Windows VERSION 0.9 LANGUAGES CXX)

if(NOT DEFINED LZMA_SDK_PATH)
	message(FATAL_ERROR "LZMA SDK path is not specified")
endif()

find_package(Windows)

add_library(wui_loader_windows INTERFACE)
target_compile_features(wui_loader_windows INTERFACE cxx_std_11)
target_link_libraries(wui_loader_windows INTERFACE ::Windows)
target_sources(wui_loader_windows
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/WUI/Graphics/Loader.hpp)
target_sources(wui_loader_windows
	INTERFACE FILE_SET lzma_dec_h TYPE HEADERS
	BASE_DIRS ${LZMA_SDK_PATH}/C
	FILES
		${LZMA_SDK_PATH}/C/Compiler.h
		${LZMA_SDK_PATH}/C/Precomp.h
		${LZMA_SDK_PATH}/C/7zWindows.h
		${LZMA_SDK_PATH}/C/7zTypes.h
		${LZMA_SDK_PATH}/C/LzmaDec.h
		${LZMA_SDK_PATH}/C/Alloc.h)
target_sources(wui_loader_windows
	INTERFACE
		${LZMA_SDK_PATH}/C/LzmaDec.c
		${LZMA_SDK_PATH}/C/Alloc.c)

add_executable(wui_loader_windows_generate)
target_compile_features(wui_loader_windows_generate PRIVATE cxx_std_17)
target_compile_definitions(wui_loader_windows_generate PRIVATE Z7_ST)
target_sources(wui_loader_windows_generate PRIVATE main.cpp)
target_sources(wui_loader_windows_generate
	PRIVATE FILE_SET lzma_enc_h TYPE HEADERS
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
		${LZMA_SDK_PATH}/C/Alloc.h)
target_sources(wui_loader_windows_generate
	PRIVATE
		${LZMA_SDK_PATH}/C/CpuArch.c
		${LZMA_SDK_PATH}/C/LzFind.c
		${LZMA_SDK_PATH}/C/LzmaEnc.c
		${LZMA_SDK_PATH}/C/Alloc.c)

add_library(WUI::Loader::Windows ALIAS wui_loader_windows)

include(GNUInstallDirs)
install(
	TARGETS
		wui_loader_windows
		wui_loader_windows_generate
	EXPORT _wuiLoaderWindowsTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILE_SET lzma_dec_h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILE_SET lzma_enc_h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(
	EXPORT _wuiLoaderWindowsTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::)
install(
	FILES ${PROJECT_SOURCE_DIR}/wuiLoaderWindowsTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
