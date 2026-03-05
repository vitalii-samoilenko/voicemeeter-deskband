include(CMakeFindDependencyMacro)

find_dependency(Windows)

include(${CMAKE_CURRENT_LIST_DIR}/_wuiLoaderWindowsTargets.cmake)
