cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Loader.Windows VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_loader_windows INTERFACE)
target_compile_features(voicemeeter_ui_loader_windows INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_loader_windows INTERFACE Windows)
target_include_directories(voicemeeter_ui_loader_windows INTERFACE include)

if(MSVC)
	if(MSVC_TOOLSET_VERSION EQUAL 145)
		set(BUILD_TOOLS_INSTALLATION_PATH "C:/Program Files/Microsoft Visual Studio/18/Community")
	else()
		message(FATAL_ERROR "Toolset is not supported")
	endif()
else()
	message(FATAL_ERROR "Compiler is not supported")
endif()

add_executable(rc.generator)
target_sources(rc.generator PRIVATE rc.cpp)
target_compile_features(rc.generator PRIVATE cxx_std_17)
add_custom_command(
	COMMAND rc.generator ${LOADER_FILES}
	OUTPUT Loader.rc
	DEPENDS rc.generator
	VERBATIM)
add_custom_command(
	COMMAND ${BUILD_TOOLS_INSTALLATION_PATH}/VC/Auxiliary/Build/vcvarsall.bat x64
		&& rc Loader.rc
	OUTPUT Loader.res
	DEPENDS Loader.rc
	VERBATIM)
target_link_options(voicemeeter_ui_loader_windows INTERFACE Loader.res)

add_executable(rc.hpp.generator)
target_sources(rc.hpp.generator PRIVATE rc.hpp.cpp)
target_compile_features(rc.hpp.generator PRIVATE cxx_std_17)
add_custom_command(
	COMMAND rc.hpp.generator ${LOADER_FILES}
	OUTPUT Voicemeeter/UI/Layouts/Loader.hpp
	DEPENDS rc.hpp.generator
	VERBATIM)
add_custom_target(run.rc.hpp.generator DEPENDS Voicemeeter/UI/Layouts/Loader.hpp)
add_dependencies(voicemeeter_ui_loader_windows run.rc.hpp.generator)

add_library(Voicemeeter::UI::Loader ALIAS voicemeeter_ui_loader_windows)

list(APPEND LAYOUTS_BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND LAYOUTS_FILES ${CMAKE_CURRENT_BINARY_DIR}/Voicemeeter/UI/Layouts/Loader.hpp)
set(LAYOUTS_BASE_DIRS ${LAYOUTS_BASE_DIRS} PARENT_SCOPE)
set(LAYOUTS_FILES ${LAYOUTS_FILES} PARENT_SCOPE)
