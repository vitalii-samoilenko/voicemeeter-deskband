cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Palette::Oklab VERSION 0.9 LANGUAGES CXX)

find_package(WUI)

add_library(voicemeeter_clients_wui_palette_oklab INTERFACE)
target_compile_features(voicemeeter_clients_wui_palette_oklab INTERFACE cxx_std_11)
target_link_libraries(voicemeeter_clients_wui_palette_oklab INTERFACE ::WUI::Palette::Oklab)
target_sources(voicemeeter_clients_wui_palette_oklab
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES include/Voicemeeter/Clients/WUI/Palette.hpp)

add_library(Voicemeeter::Clients::WUI::Palette::Oklab ALIAS voicemeeter_clients_wui_palette_oklab)
