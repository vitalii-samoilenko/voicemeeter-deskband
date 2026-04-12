if(NOT DEFINED MSVC_TOOLSET_PATH)
	message(FATAL_ERROR "MSVC toolset path is not specified")
endif()

get_property(LOADER_FILES
	TARGET ::wui_loader
	PROPERTY RESOURCE)

add_custom_command(
	COMMAND ::wui_loader_windows_generate ${LOADER_FILES}
	OUTPUT
		Loader.rc
		WUI/Layouts/Loader.hpp
	DEPENDS ::wui_loader_windows_generate
	VERBATIM)
add_custom_command(
	COMMAND ${MSVC_TOOLSET_PATH}/Auxiliary/Build/vcvarsall.bat x64
		&& rc Loader.rc
	OUTPUT Loader.res
	DEPENDS ${PROJECT_BINARY_DIR}/Loader.rc
	VERBATIM)

target_link_libraries(::_wui_loader_windows INTERFACE ${PROJECT_BINARY_DIR}/Loader.res)

target_sources(::wui_layouts
	INTERFACE FILE_SET HEADERS
	BASE_DIRS ${PROJECT_BINARY_DIR}
	FILES ${PROJECT_BINARY_DIR}/WUI/Layouts/Loader.hpp)
