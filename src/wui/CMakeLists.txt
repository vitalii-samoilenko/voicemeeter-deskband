cmake_minimum_required(VERSION 3.25.1)

project(WUI::Voicemeeter VERSION 0.9)

add_subdirectory(atlas)
add_subdirectory(controls)

include(${WUI_DIR}/loaderBind.cmake)
