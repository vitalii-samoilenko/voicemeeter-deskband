cmake_minimum_required(VERSION 3.25.1)

project(WUI::Loader VERSION 0.9)

if(WUI_LOADER STREQUAL Windows)
	add_subdirectory(windows)
else()
	message(FATAL_ERROR "Resource manager is not supported")
endif()
