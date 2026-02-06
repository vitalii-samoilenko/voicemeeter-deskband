cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.DeskBandit VERSION 0.9 LANGUAGES CXX)

add_executable(Voicemeeter.DeskBandit)
target_sources(Voicemeeter.DeskBandit PRIVATE main.cpp)
target_compile_features(Voicemeeter.DeskBandit PRIVATE cxx_std_17)
target_link_libraries(Voicemeeter.DeskBandit PRIVATE
		wheel
		Windows
		Voicemeeter
		Voicemeeter::Clients::Remote
		Voicemeeter::Clients::UI::Controls
		Voicemeeter::Clients::UI::Graphics
		Voicemeeter::Clients::UI::Loader
		Voicemeeter::Clients::UI::Palette
		Voicemeeter::Clients::UI::Scene)
target_include_directories(Voicemeeter.DeskBandit PRIVATE include)

if(MSVC)
	if(MSVC_TOOLSET_VERSION EQUAL 145)
		set(BUILD_TOOLS_INSTALLATION_PATH "C:/Program Files/Microsoft Visual Studio/18/Community")
	else()
		message(FATAL_ERROR "Toolset is not supported")
	endif()
else()
	message(FATAL_ERROR "Compiler is not supported")
endif()

add_custom_command(
	COMMAND ${BUILD_TOOLS_INSTALLATION_PATH}/VC/Auxiliary/Build/vcvarsall.bat x64
		&& mc -b -c ${CMAKE_CURRENT_SOURCE_DIR}/Messages.mc
	OUTPUT
		Messages.h
		Messages_English_US.bin
	DEPENDS Messages.mc
	VERBATIM)
target_sources(Voicemeeter.DeskBandit PRIVATE
	FILE_SET generated TYPE HEADERS
	BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
	FILES ${CMAKE_CURRENT_BINARY_DIR}/Messages.h)

add_custom_command(
	COMMAND ${BUILD_TOOLS_INSTALLATION_PATH}/VC/Auxiliary/Build/vcvarsall.bat x64
		&& rc ${CMAKE_CURRENT_SOURCE_DIR}/Resources.rc
	OUTPUT Resources.res
	DEPENDS
		Messages_English_US.bin
		Resources.rc
	VERBATIM)
target_link_options(Voicemeeter.DeskBandit PRIVATE Resources.res)

install(TARGETS Voicemeeter.DeskBandit)
