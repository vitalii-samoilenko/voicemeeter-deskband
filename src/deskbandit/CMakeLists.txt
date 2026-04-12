cmake_minimum_required(VERSION 3.25.1)

project(DeskBandit VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(Windows)

add_executable(deskbandit WIN32)
target_compile_features(deskbandit PRIVATE cxx_std_17)
target_link_libraries(deskbandit PRIVATE
		::WSTD
		::Windows::API
		Voicemeeter
		Voicemeeter::Clients::VBVMR
		Voicemeeter::Clients::WUI::Controls
		Voicemeeter::Clients::WUI::Graphics
		Voicemeeter::Clients::WUI::Loader
		Voicemeeter::Clients::WUI::Palette
		Voicemeeter::Clients::WUI::Scene)
target_sources(deskbandit
	PRIVATE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/DeskBandit.hpp
		include/ErrorMessageBox.hpp)
target_sources(deskbandit PRIVATE main.cpp)

if(NOT DEFINED MSVC_TOOLSET_PATH)
	message(FATAL_ERROR "MSVC toolset path is not specified")
endif()

add_custom_command(
	COMMAND ${MSVC_TOOLSET_PATH}/Auxiliary/Build/vcvarsall.bat x64
		&& mc -b -c ${PROJECT_SOURCE_DIR}/Messages.mc
	OUTPUT
		Messages.h
		Messages.rc
	DEPENDS Messages.mc
	VERBATIM)
add_custom_command(
	COMMAND ${MSVC_TOOLSET_PATH}/Auxiliary/Build/vcvarsall.bat x64
		&& rc Messages.rc
	OUTPUT Messages.res
	DEPENDS ${PROJECT_BINARY_DIR}/Messages.rc
	VERBATIM)
add_custom_command(
	COMMAND ${MSVC_TOOLSET_PATH}/Auxiliary/Build/vcvarsall.bat x64
		&& rc /fo ${PROJECT_BINARY_DIR}/Version.res
		${PROJECT_SOURCE_DIR}/Version.rc
	OUTPUT Version.res
	DEPENDS Version.rc
	VERBATIM)

target_link_libraries(deskbandit
	PRIVATE
	${PROJECT_BINARY_DIR}/Messages.res
	${PROJECT_BINARY_DIR}/Version.res)

target_sources(deskbandit
	PRIVATE FILE_SET messages_h TYPE HEADERS
	BASE_DIRS ${PROJECT_BINARY_DIR}
	FILES ${PROJECT_BINARY_DIR}/Messages.h)

include(GNUInstallDirs)
install(
	TARGETS deskbandit
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
