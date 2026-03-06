cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Graphics::Vulkan VERSION 0.9 LANGUAGES CXX)

find_package(WUI)

add_library(voicemeeter_clients_wui_graphics_vulkan INTERFACE)
target_compile_features(voicemeeter_clients_wui_graphics_vulkan INTERFACE cxx_std_11)
target_link_libraries(voicemeeter_clients_wui_graphics_vulkan INTERFACE
	::WUI::Scene
	::WUI::Graphics::Vulkan
	WUI::Atlas)
target_sources(voicemeeter_clients_wui_graphics_vulkan
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Voicemeeter/Clients/WUI/Canvas.hpp
		include/Voicemeeter/Clients/WUI/Surface.hpp)

add_library(Voicemeeter::Clients::WUI::Graphics::D3D12 ALIAS voicemeeter_clients_wui_graphics_d3d12)
