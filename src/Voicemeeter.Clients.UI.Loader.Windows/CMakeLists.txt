cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Clients.UI.Loader.Windows VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_clients_ui_loader_windows INTERFACE)
target_compile_features(voicemeeter_clients_ui_loader_windows INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_clients_ui_loader_windows INTERFACE
	Windows
	Voicemeeter::UI::Loader)
target_include_directories(voicemeeter_clients_ui_loader_windows INTERFACE include)

add_library(Voicemeeter::Clients::UI::Loader ALIAS voicemeeter_clients_ui_loader_windows)
