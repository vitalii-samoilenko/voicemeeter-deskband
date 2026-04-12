cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::WUI::Graphics VERSION 0.9)

if(WUI_GRAPHICS STREQUAL D3D12)
	add_subdirectory(d3d12)
	add_library(Voicemeeter::Clients::WUI::Graphics ALIAS _voicemeeter_clients_wui_graphics_d3d12)
elseif(WUI_GRAPHICS STREQUAL Vulkan)
	add_subdirectory(vulkan)
	add_library(Voicemeeter::Clients::WUI::Graphics ALIAS _voicemeeter_clients_wui_graphics_vulkan)
else()
	message(FATAL_ERROR "Rendering backend is not supported")
endif()
