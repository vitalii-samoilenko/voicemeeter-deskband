cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Controls::Cherry VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(WUI)

add_library(_voicemeeter_clients_wui_controls_cherry INTERFACE)
target_compile_features(_voicemeeter_clients_wui_controls_cherry INTERFACE cxx_std_17)
target_link_libraries(_voicemeeter_clients_wui_controls_cherry INTERFACE
	::WSTD
	::WUI::Scene
	WUI::Controls::Cherry
	Voicemeeter)
target_sources(_voicemeeter_clients_wui_controls_cherry
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Voicemeeter/Clients/WUI/_/Controls.hpp
		include/Voicemeeter/Clients/WUI/Composition.hpp
		include/Voicemeeter/Clients/WUI/Theme.hpp)

add_library(Voicemeeter::Clients::WUI::Controls::Cherry ALIAS _voicemeeter_clients_wui_controls_cherry)
add_library(Voicemeeter::Clients::WUI::Controls ALIAS _voicemeeter_clients_wui_controls_cherry)
