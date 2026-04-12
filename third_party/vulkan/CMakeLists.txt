cmake_minimum_required(VERSION 3.25.1)

project(Vulkan VERSION 0.9 LANGUAGES CXX)

add_library(_vulkan_api INTERFACE)
target_compile_features(_vulkan_api INTERFACE cxx_std_11)
target_link_libraries(_vulkan_api INTERFACE vulkan)
target_sources(_vulkan_api
	INTERFACE FILE_SET HEADERS
	BASE_DIRS include
	FILES
		include/Vulkan/Error.hpp
		include/Vulkan/API.hpp)

add_library(API ALIAS _vulkan_api)

include(GNUInstallDirs)
install(
	TARGETS _vulkan_api
	EXPORT vulkanTargets)
install(
	EXPORT vulkanTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vulkan
	NAMESPACE ::Vulkan)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/VulkanConfig.cmake.in
	${PROJECT_BINARY_DIR}/cmake/VulkanConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vulkan)
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/cmake/VulkanConfigVersion.cmake
	VERSION 0.9.0
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${PROJECT_BINARY_DIR}/cmake/VulkanConfig.cmake
		${PROJECT_BINARY_DIR}/cmake/VulkanConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vulkan)
