cmake_minimum_required(VERSION 3.25.1)

project(WUI::Loader VERSION 0.9)

add_library(wui_loader INTERFACE)

if(WUI_LOADER STREQUAL Windows)
	add_subdirectory(windows)
	add_library(WUI::Loader ALIAS _wui_loader_windows)
else()
	message(FATAL_ERROR "Resource manager is not supported")
endif()

include(GNUInstallDirs)
install(
	TARGETS wui_loader
	EXPORT _loaderTargets
	RESOURCE DESTINATION ${CMAKE_INSTALL_DATADIR})
install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/loaderTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
