cmake_minimum_required(VERSION 3.25.1)

project(WUI::Graphics::Vulkan VERSION 0.9 LANGUAGES CXX)

add_library(wui_graphics_vulkan INTERFACE)
target_compile_features(wui_graphics_vulkan INTERFACE cxx_std_11)
target_link_libraries(wui_graphics_vulkan INTERFACE estd wheel WUI::Layouts)
target_include_directories(wui_graphics_vulkan INTERFACE include)
target_link_options(wui_graphics_vulkan INTERFACE vulkan-1.lib)

list(APPEND WUI_LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Vertex.glsl)
list(APPEND WUI_LOADER_FILES ${PROJECT_SOURCE_DIR}/export/MSDF.glsl)
list(APPEND WUI_LOADER_FILES ${PROJECT_SOURCE_DIR}/export/Blender.glsl)
set(WUI_LOADER_FILES ${WUI_LOADER_FILES} PARENT_SCOPE)

add_library(WUI::Graphics ALIAS wui_graphics_vulkan)
