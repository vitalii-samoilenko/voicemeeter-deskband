cmake_minimum_required(VERSION 3.25.1)

project(WUI::Palette::Oklab VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(Oklab)

add_library(wui_palette_oklab INTERFACE)
target_compile_features(wui_palette_oklab INTERFACE cxx_std_11)
target_link_libraries(wui_palette_oklab
	INTERFACE
		::WSTD
		::Oklab)
target_sources(wui_palette_oklab
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/WUI/Graphics/Palette.hpp)

add_library(WUI::Palette::Oklab ALIAS wui_palette_oklab)

include(GNUInstallDirs)
install(
	TARGETS wui_palette_oklab
	EXPORT _wuiPaletteOklabTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _wuiPaletteOklabTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::)
install(
	FILES ${PROJECT_SOURCE_DIR}/wuiPaletteOklabTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
