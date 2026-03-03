cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.Clients.Remote VERSION 0.9 LANGUAGES CXX)

add_library(voicemeeter_clients_remote INTERFACE)
target_compile_features(voicemeeter_clients_remote INTERFACE cxx_std_17)
target_link_libraries(voicemeeter_clients_remote INTERFACE
	wheel
	Voicemeeter)
target_include_directories(voicemeeter_clients_remote INTERFACE include)

add_library(Voicemeeter::Clients::Remote ALIAS voicemeeter_clients_remote)
