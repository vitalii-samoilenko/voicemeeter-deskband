cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Controls.Cherry VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_controls_cherry INTERFACE)
target_compile_features(voicemeeter_ui_controls_cherry INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_controls_cherry INTERFACE
	wheel
	Voicemeeter::UI::Layouts)
target_include_directories(voicemeeter_ui_controls_cherry INTERFACE include)

if(CONTROLS STREQUAL Cherry)
	add_library(Voicemeeter::UI::Controls ALIAS voicemeeter_ui_controls_cherry)
endif()
