cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Palette.Oklab VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_ui_palette_oklab INTERFACE)
add_library(Voicemeeter::UI::Palette::Oklab ALIAS voicemeeter_ui_palette_oklab)
target_compile_features(voicemeeter_ui_palette_oklab INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_ui_palette_oklab INTERFACE wheel)
target_include_directories(voicemeeter_ui_palette_oklab INTERFACE include)
