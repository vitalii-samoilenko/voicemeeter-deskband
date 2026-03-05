cmake_minimum_required(VERSION 3.25.1)

project(WUI::Scene VERSION 0.9 LANGUAGES CXX)

find_package(WSTD)

add_library(wui_scene INTERFACE)
target_compile_features(wui_scene INTERFACE cxx_std_17)
target_link_libraries(wui_scene INTERFACE ::WSTD)
target_sources(wui_scene
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/WUI/Adapters/Canvas.hpp
		include/WUI/Adapters/Glyph.hpp
		include/WUI/Adapters/Graphics/Animated.hpp
		include/WUI/Adapters/IBundle.hpp
		include/WUI/Adapters/IComponent.hpp
		include/WUI/Adapters/Interactivity/Controllers/Circular.hpp
		include/WUI/Adapters/Interactivity/Transparent.hpp
		include/WUI/Adapters/State.hpp
		include/WUI/Decorators/HitTest.hpp
		include/WUI/Decorators/Interactivity/Carousel.hpp
		include/WUI/Decorators/Interactivity/Grab.hpp
		include/WUI/Decorators/Interactivity/Point.hpp
		include/WUI/Decorators/Padding.hpp
		include/WUI/Focus.hpp
		include/WUI/Graphics/Queue.hpp
		include/WUI/Graphics/Stopwatch.hpp
		include/WUI/Graphics/Toolkit.hpp
		include/WUI/Panels/Stack.hpp
		include/WUI/Policies/Orientation/Directions/Axis.hpp
		include/WUI/Policies/Size/Scales/PreserveRatio.hpp
		include/WUI/Scene.hpp
		include/WUI/Trackers/Focus.hpp)

add_library(WUI::Scene ALIAS wui_scene)

include(GNUInstallDirs)
install(
	TARGETS wui_scene
	EXPORT _wuiSceneTargets
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
	EXPORT _wuiSceneTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI
	NAMESPACE ::)
install(
	FILES ${PROJECT_SOURCE_DIR}/wuiSceneTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/WUI)
