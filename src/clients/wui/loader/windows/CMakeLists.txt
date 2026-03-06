cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Loader::Windows VERSION 0.9 LANGUAGES CXX)

find_package(WUI)

add_library(voicemeeter_clients_wui_loader_windows INTERFACE)
target_compile_features(voicemeeter_clients_wui_loader_windows INTERFACE cxx_std_11)
target_link_libraries(voicemeeter_clients_wui_loader_windows INTERFACE ::WUI::Loader::Windows)
target_sources(voicemeeter_clients_wui_loader_windows
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/Voicemeeter/Clients/WUI/Loader.hpp)

add_library(Voicemeeter::Clients::WUI::Loader::Windows ALIAS voicemeeter_clients_wui_loader_windows)
