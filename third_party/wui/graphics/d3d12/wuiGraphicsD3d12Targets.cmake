include(CMakeFindDependencyMacro)

find_dependency(WSTD)
find_dependency(Windows)

include(${CMAKE_CURRENT_LIST_DIR}/_wuiGraphicsD3d12Targets.cmake)
