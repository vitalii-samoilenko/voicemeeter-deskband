include(CMakeFindDependencyMacro)

find_dependency(WSTD)
find_dependency(Oklab)

include(${CMAKE_CURRENT_LIST_DIR}/_paletteOklabTargets.cmake)
