cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Clients.UI.Scene VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_clients_ui_scene INTERFACE)
target_compile_features(voicemeeter_clients_ui_scene INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_clients_ui_scene INTERFACE Voicemeeter::UI)
target_include_directories(voicemeeter_clients_ui_scene INTERFACE include)

add_library(Voicemeeter::Clients::UI::Scene ALIAS voicemeeter_clients_ui_scene)
