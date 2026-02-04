cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.DeskBandit VERSION 0.9 LANGUAGES CXX)

add_executable(Voicemeeter.DeskBandit main.cpp)
if(MSVC)
	if(MSVC_TOOLSET_VERSION EQUAL 143)
		set(BUILD_TOOLS_INSTALLATION_PATH "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools")
	endif()
	else()
		message(FATAL_ERROR "Toolset is not supported")
	endif()
	execute_process(COMMAND ${BUILD_TOOLS_INSTALLATION_PATH}/VC/Auxiliary/Build/vcvarsall.bat x64 && rc
			/fo ${CMAKE_CURRENT_BINARY_DIR}/Resources.res
			${PROJECT_SOURCE_DIR}/Resources.rc
		COMMAND_ERROR_IS_FATAL ANY)
	target_link_options(Voicemeeter.DeskBandit PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Resources.res)
endif()
target_compile_features(Voicemeeter.DeskBandit PRIVATE cxx_std_17)
target_link_libraries(Voicemeeter.DeskBandit
	PRIVATE
		wheel
		Windows
		Voicemeeter
		Voicemeeter::Clients::Remote
		Voicemeeter::Clients::UI::Controls
		Voicemeeter::Clients::UI::Graphics
		Voicemeeter::Clients::UI::Palette
		Voicemeeter::Clients::UI::Scene)
target_include_directories(Voicemeeter.DeskBandit PRIVATE include)

install(TARGETS Voicemeeter.DeskBandit)
