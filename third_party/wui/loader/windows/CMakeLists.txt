cmake_minimum_required(VERSION 3.25.1)

project(WUI::Loader::Windows VERSION 0.9 LANGUAGES CXX)

find_package(Windows REQUIRED COMPONENTS API)
find_package(LZMA
	REQUIRED COMPONENTS
		Enc
		Dec)

add_library(_wui_loader_windows INTERFACE)
target_compile_features(_wui_loader_windows INTERFACE cxx_std_11)
target_link_libraries(_wui_loader_windows
	INTERFACE
		::Windows::API
		::LZMA::Dec)
target_sources(_wui_loader_windows
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/WUI/Graphics/Loader.hpp)

add_executable(wui_loader_windows_generate)
target_compile_features(wui_loader_windows_generate PRIVATE cxx_std_17)
target_link_libraries(wui_loader_windows_generate PRIVATE ::LZMA::Enc)
target_compile_definitions(wui_loader_windows_generate PRIVATE Z7_ST)
target_sources(wui_loader_windows_generate PRIVATE main.cpp)

add_library(Loader::Windows ALIAS _wui_loader_windows)

include(GNUInstallDirs)
install(
	TARGETS
		_wui_loader_windows
		wui_loader_windows_generate
	EXPORT _loaderWindowsTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(
	EXPORT _loaderWindowsTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::WUI)
install(
	FILES ${PROJECT_SOURCE_DIR}/cmake/loaderWindowsTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
