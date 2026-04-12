cmake_minimum_required(VERSION 3.25.1)

project(WUI::Palette::Oklab VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(Oklab)

add_library(_wui_palette_oklab INTERFACE)
target_compile_features(_wui_palette_oklab INTERFACE cxx_std_11)
target_link_libraries(_wui_palette_oklab
	INTERFACE
		::WSTD
		::Oklab)
target_sources(_wui_palette_oklab
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/WUI/Graphics/Palette.hpp)

add_library(Palette::Oklab ALIAS _wui_palette_oklab)

include(GNUInstallDirs)
install(
	TARGETS _wui_palette_oklab
	EXPORT _paletteOklabTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _paletteOklabTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::WUI)
install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/paletteOklabTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
