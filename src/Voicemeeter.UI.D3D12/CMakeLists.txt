cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.D3D12 VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_d3d12 INTERFACE)
add_library(Voicemeeter::UI::D3D12 ALIAS voicemeeter_ui_d3d12)
target_compile_features(voicemeeter_ui_d3d12 INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_d3d12
	INTERFACE
		wheel
		Windows
		Voicemeeter::UI::Atlas)
target_include_directories(voicemeeter_ui_d3d12 INTERFACE include)
