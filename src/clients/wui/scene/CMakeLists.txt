cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Scene VERSION 0.9 LANGUAGES CXX)

find_package(WUI)

add_library(_voicemeeter_clients_wui_scene INTERFACE)
target_compile_features(_voicemeeter_clients_wui_scene INTERFACE cxx_std_11)
target_link_libraries(_voicemeeter_clients_wui_scene INTERFACE ::WUI::Scene)
target_sources(_voicemeeter_clients_wui_scene
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Voicemeeter/Clients/WUI/FocusTracker.hpp
		include/Voicemeeter/Clients/WUI/Scene.hpp)

add_library(Voicemeeter::Clients::WUI::Scene ALIAS _voicemeeter_clients_wui_scene)
