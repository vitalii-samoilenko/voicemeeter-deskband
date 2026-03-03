cmake_minimum_required(VERSION 3.25.1)

project(WUI VERSION 0.9)

set(WUI_GRAPHICS "" CACHE STRING
	"Choose rendering backend: D3D12, Vulkan")
set(WUI_PALETTE "" CACHE STRING
	"Choose color space for interpolation: Oklab")
set(WUI_LOADER "" CACHE STRING
	"Choose resource manager: Windows")

add_subdirectory(graphics)
add_subdirectory(palette)
add_subdirectory(scene)
add_subdirectory(loader)
add_subdirectory(layouts)
