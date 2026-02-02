cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Clients.UI.Graphics.D3D12 VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_clients_ui_graphics_d3d12 INTERFACE)
if(GRAPHICS STREQUAL D3D12)
	add_library(Voicemeeter::Clients::UI::Graphics ALIAS voicemeeter_clients_ui_graphics_d3d12)
endif()
target_compile_features(voicemeeter_clients_ui_graphics_d3d12 INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_clients_ui_graphics_d3d12
	INTERFACE
		Windows
		Voicemeeter::UI
		Voicemeeter::UI::Graphics)
target_include_directories(voicemeeter_clients_ui_graphics_d3d12 INTERFACE include)
