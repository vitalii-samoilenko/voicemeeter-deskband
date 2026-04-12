cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Palette VERSION 0.9)

if(WUI_PALETTE STREQUAL Oklab)
	add_subdirectory(oklab)
	add_library(Voicemeeter::Clients::WUI::Palette ALIAS _voicemeeter_clients_wui_palette_oklab)
else()
	message(FATAL_ERROR "Color space is not supported")
endif()
