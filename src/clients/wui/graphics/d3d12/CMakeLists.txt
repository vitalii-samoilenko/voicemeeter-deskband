cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Graphics::D3D12 VERSION 0.9 LANGUAGES CXX)

find_package(WUI)

add_library(_voicemeeter_clients_wui_graphics_d3d12 INTERFACE)
target_compile_features(_voicemeeter_clients_wui_graphics_d3d12 INTERFACE cxx_std_11)
target_link_libraries(_voicemeeter_clients_wui_graphics_d3d12 INTERFACE
	::WUI::Scene
	::WUI::Graphics::D3D12
	WUI::Atlas)
target_sources(_voicemeeter_clients_wui_graphics_d3d12
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Voicemeeter/Clients/WUI/Canvas.hpp
		include/Voicemeeter/Clients/WUI/Surface.hpp)

add_library(Voicemeeter::Clients::WUI::Graphics::D3D12 ALIAS _voicemeeter_clients_wui_graphics_d3d12)
