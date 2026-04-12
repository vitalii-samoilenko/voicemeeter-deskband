cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)

add_library(_voicemeeter INTERFACE)
target_compile_features(_voicemeeter INTERFACE cxx_std_11)
target_link_libraries(_voicemeeter INTERFACE ::WSTD)
target_sources(_voicemeeter
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Voicemeeter/Adapters/Amplifier.hpp
		include/Voicemeeter/Adapters/Mixer.hpp
		include/Voicemeeter/Adapters/Multiclient/Amplifier.hpp
		include/Voicemeeter/Adapters/Multiclient/Channel.hpp
		include/Voicemeeter/Adapters/Multiclient/Mixer.hpp
		include/Voicemeeter/Adapters/Multiclient/Mute.hpp
		include/Voicemeeter/Adapters/Multiclient/Network.hpp
		include/Voicemeeter/Adapters/Multiclient/Token.hpp
		include/Voicemeeter/Adapters/Mute.hpp
		include/Voicemeeter/Adapters/Network.hpp
		include/Voicemeeter/Bus.hpp
		include/Voicemeeter/Channel.hpp
		include/Voicemeeter/Cherry.hpp)

add_library(Voicemeeter ALIAS _voicemeeter)
