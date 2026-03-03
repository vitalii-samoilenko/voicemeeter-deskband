cmake_minimum_required(VERSION 3.25.1)

project(WUI::Loader::Windows VERSION 0.9 LANGUAGES CXX)

add_library(wui_loader_windows INTERFACE)
target_compile_features(wui_loader_windows INTERFACE cxx_std_11)
target_link_libraries(wui_loader_windows INTERFACE Windows)
target_include_directories(wui_loader_windows INTERFACE include)

if(NOT DEFINED MSVC_TOOLSET_PATH)
	message(FATAL_ERROR "MSVC toolset path is not specified")
endif()
if(NOT DEFINED LZMA_SDK_PATH)
	message(FATAL_ERROR "LZMA SDK path is not specified")
endif()

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

add_executable(rc.pre.compression)
target_sources(rc.pre.compression PRIVATE compression.cpp)
target_sources(rc.pre.compression
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
target_sources(rc.pre.compression
	PRIVATE
		${LZMA_SDK_PATH}/C/CpuArch.c
		${LZMA_SDK_PATH}/C/LzFind.c
		${LZMA_SDK_PATH}/C/LzmaEnc.c
		${LZMA_SDK_PATH}/C/Alloc.c)
target_compile_features(rc.pre.compression PRIVATE cxx_std_17)
target_compile_definitions(rc.pre.compression PRIVATE Z7_ST)
add_custom_command(
	COMMAND rc.pre.compression ${LOADER_FILES}
	OUTPUT rc.pre.compression.done
	DEPENDS rc.pre.compression
	VERBATIM)
add_executable(rc.pre.definition)
target_sources(rc.pre.definition PRIVATE rc.pre.definition.cpp)
target_compile_features(rc.pre.definition PRIVATE cxx_std_17)
add_custom_command(
	COMMAND rc.pre.definition ${LOADER_FILES}
	OUTPUT Loader.rc
	DEPENDS rc.pre.definition
	VERBATIM)
add_executable(rc.pre.description)
target_sources(rc.pre.description PRIVATE rc.pre.description.cpp)
target_compile_features(rc.pre.description PRIVATE cxx_std_17)
add_custom_command(
	COMMAND rc.pre.description ${LOADER_FILES}
	OUTPUT WUI/Layouts/Loader.hpp
	DEPENDS rc.pre.description
	VERBATIM)
add_custom_command(
	COMMAND ${MSVC_TOOLSET_PATH}/Auxiliary/Build/vcvarsall.bat x64
		&& rc Loader.rc
	OUTPUT Loader.res
	DEPENDS
		${PROJECT_BINARY_DIR}/rc.pre.compression.done
		${PROJECT_BINARY_DIR}/Loader.rc
	VERBATIM)
add_custom_target(run.rc
	DEPENDS
	${PROJECT_BINARY_DIR}/Loader.res
	${PROJECT_BINARY_DIR}/WUI/Layouts/Loader.hpp)
add_dependencies(wui_loader_windows run.rc)

target_link_options(wui_loader_windows INTERFACE ${PROJECT_BINARY_DIR}/Loader.res)

list(APPEND WUI_LAYOUTS_BASE_DIRS ${PROJECT_BINARY_DIR})
list(APPEND WUI_LAYOUTS_FILES ${PROJECT_BINARY_DIR}/WUI/Layouts/Loader.hpp)
set(WUI_LAYOUTS_BASE_DIRS ${WUI_LAYOUTS_BASE_DIRS} PARENT_SCOPE)
set(WUI_LAYOUTS_FILES ${WUI_LAYOUTS_FILES} PARENT_SCOPE)

add_library(WUI::Loader ALIAS wui_loader_windows)
