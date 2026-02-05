cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Clients.UI.Controls.Cherry VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_clients_ui_controls_cherry INTERFACE)
target_compile_features(voicemeeter_clients_ui_controls_cherry INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_clients_ui_controls_cherry INTERFACE
	wheel
	Voicemeeter
	Voicemeeter::UI
	Voicemeeter::UI::Controls)
target_include_directories(voicemeeter_clients_ui_controls_cherry INTERFACE include)

if(CONTROLS STREQUAL Cherry)
	add_library(Voicemeeter::Clients::UI::Controls ALIAS voicemeeter_clients_ui_controls_cherry)
endif()
