cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::DeskBand VERSION 0.9)

set(WUI_LOADER Windows CACHE STRING)
set(WUI_PALETTE Oklab CACHE STRING)
set(WUI_GRAPHICS D3D12 CACHE STRING)
set(WUI_ATLAS_GENERATE ON CACHE BOOL)

if(MSVC)
  add_compile_options(/bigobj)
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
  else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
  endif()
  add_compile_options(/GR-)
elseif(LINUX)
  set(CMAKE_EXE_LINKER_FLAGS "--static -static-libgcc -static-libstdc++")
endif()

	if(NOT DEFINED MSVC_TOOLSET_PATH)
		message(FATAL_ERROR "MSVC toolset path is not specified")
	endif()

	get_property(LOADER_FILES
		TARGET wui_loader
		PROPERTY RESOURCE)

	add_custom_command(
		COMMAND rc.pre.compress ${LOADER_FILES}
		OUTPUT rc.pre.compress.done
		DEPENDS rc.pre.compress
		VERBATIM)
	add_custom_command(
		COMMAND rc.pre.define ${LOADER_FILES}
		OUTPUT Loader.rc
		DEPENDS rc.pre.define
		VERBATIM)
	add_custom_command(
		COMMAND rc.pre.describe ${LOADER_FILES}
		OUTPUT WUI/Layouts/Loader.hpp
		DEPENDS rc.pre.describe
		VERBATIM)
	add_custom_command(
		COMMAND ${MSVC_TOOLSET_PATH}/Auxiliary/Build/vcvarsall.bat x64
			&& rc Loader.rc
		OUTPUT Loader.res
		DEPENDS
			${PROJECT_BINARY_DIR}/rc.pre.compress.done
			${PROJECT_BINARY_DIR}/Loader.rc
		VERBATIM)

	target_link_libraries(wui_loader_windows INTERFACE ${PROJECT_BINARY_DIR}/Loader.res)

	target_sources(wui_layouts
		INTERFACE FILE_SET HEADERS
		BASE_DIRS ${PROJECT_BINARY_DIR}
		FILES ${PROJECT_BINARY_DIR}/WUI/Layouts/Loader.hpp)

add_subdirectory(src/wui)
add_subdirectory(third_party)
add_subdirectory(src/voicemeeter)
add_subdirectory(src/clients)
add_subdirectory(src/deskbandit)
