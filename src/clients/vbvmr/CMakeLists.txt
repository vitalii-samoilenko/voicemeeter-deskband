cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter::Clients::VBVMR VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(Windows)
find_package(VBVMR)

add_library(_voicemeeter_clients_vbvmr INTERFACE)
target_compile_features(_voicemeeter_clients_vbvmr INTERFACE cxx_std_17)
target_link_libraries(_voicemeeter_clients_vbvmr
	INTERFACE
		::WSTD
		::Windows::API
		::VBVMR
		Voicemeeter)
target_sources(_voicemeeter_clients_vbvmr
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Voicemeeter/Clients/_/VBVMR.hpp
		include/Voicemeeter/Clients/VBVMR.hpp)

add_library(Voicemeeter::Clients::VBVMR ALIAS _voicemeeter_clients_vbvmr)
