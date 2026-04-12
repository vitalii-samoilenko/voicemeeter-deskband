include(CMakeFindDependencyMacro)

find_dependency(WSTD)
find_dependency(Vulkan)

include(${CMAKE_CURRENT_LIST_DIR}/_graphicsVulkanTargets.cmake)
