cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Clients.UI.Palette.Oklab VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_clients_ui_palette_oklab INTERFACE)
if(PALETTE STREQUAL Oklab)
	add_library(Voicemeeter::Clients::UI::Palette ALIAS voicemeeter_clients_ui_palette_oklab)
endif()
target_compile_features(voicemeeter_clients_ui_palette_oklab INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_clients_ui_palette_oklab INTERFACE Voicemeeter::UI::Palette)
target_include_directories(voicemeeter_clients_ui_palette_oklab INTERFACE include)
