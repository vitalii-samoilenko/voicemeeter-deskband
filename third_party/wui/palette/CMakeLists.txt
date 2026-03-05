cmake_minimum_required(VERSION 3.25.1)

project(WUI::Palette VERSION 0.9)

if(WUI_PALETTE STREQUAL Oklab)
	add_subdirectory(oklab)
	add_library(WUI::Palette ALIAS wui_palette_oklab)
else()
	message(FATAL_ERROR "Color space is not supported")
endif()

include(GNUInstallDirs)
install(
	FILES ${PROJECT_SOURCE_DIR}/wuiPaletteTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
