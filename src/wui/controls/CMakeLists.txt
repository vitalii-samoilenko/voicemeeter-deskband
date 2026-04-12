cmake_minimum_required(VERSION 3.25.1)

project(WUI::Controls::Cherry VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)
find_package(WUI)

add_library(_wui_controls_cherry INTERFACE)
target_compile_features(_wui_controls_cherry INTERFACE cxx_std_17)
target_link_libraries(_wui_controls_cherry
	INTERFACE
		::WSTD
		::WUI::Layouts)
target_sources(_wui_controls_cherry
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/WUI/Adapters/Interactivity/Controllers/Plug.hpp
		include/WUI/Adapters/Interactivity/Controllers/StripKnob.hpp
		include/WUI/Decorators/Interactivity/StripKnob.hpp
		include/WUI/Graphics/Bundles/Knob.hpp
		include/WUI/Graphics/Bundles/Plug.hpp
		include/WUI/Graphics/Bundles/Vban.hpp
		include/WUI/Graphics/Theme.hpp
		include/WUI/Policies/Control/Updates/Animated/Plug.hpp
		include/WUI/Policies/Control/Updates/Animated/StripKnob.hpp
		include/WUI/Policies/Control/Updates/Animated/Vban.hpp
		include/WUI/Policies/Control/Updates/Plug.hpp
		include/WUI/Policies/Control/Updates/StripKnob.hpp
		include/WUI/Policies/Control/Updates/Vban.hpp
		include/WUI/States/Plug.hpp
		include/WUI/States/StripKnob.hpp)

add_library(WUI::Controls::Cherry ALIAS _wui_controls_cherry)
add_library(WUI::Controls ALIAS _wui_controls_cherry)
