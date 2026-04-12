cmake_minimum_required(VERSION 3.25.1)

project(WUI::Layouts VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)

add_library(wui_layouts INTERFACE)
target_compile_features(wui_layouts INTERFACE cxx_std_11)
target_link_libraries(wui_layouts INTERFACE ::WSTD)

add_library(WUI::Layouts ALIAS wui_layouts)

include(GNUInstallDirs)
install(
	TARGETS wui_layouts
	EXPORT _layoutsTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _layoutsTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::)
install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/layoutsTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
