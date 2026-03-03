cmake_minimum_required(VERSION 3.25.1)

project(WUI::Palette VERSION 0.9)

if(WUI_PALETTE STREQUAL Oklab)
	add_subdirectory(oklab)
else()
	message(FATAL_ERROR "Color space is not supported")
endif()
