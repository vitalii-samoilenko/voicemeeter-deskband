cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Loader VERSION 0.9)

if(WUI_LOADER STREQUAL Windows)
	add_subdirectory(windows)
	add_library(Voicemeeter::Clients::WUI::Loader ALIAS voicemeeter_clients_wui_loader_windows)
else()
	message(FATAL_ERROR "Resource manager is not supported")
endif()
