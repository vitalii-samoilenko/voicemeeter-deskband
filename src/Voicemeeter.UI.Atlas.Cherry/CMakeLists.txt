cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter.UI.Atlas.Cherry VERSION 0.9 LANGUAGES CXX)

list(APPEND LOADER_ITEMS ${PROJECT_SOURCE_DIR}/export/Atlas.bit)
list(APPEND LAYOUTS ${PROJECT_SOURCE_DIR}/export/Atlas.hpp)

if(ATLAS STREQUAL Cherry)
	set(LOADER_ITEMS ${LOADER_ITEMS} PARENT_SCOPE)
	set(LAYOUTS ${LAYOUTS} PARENT_SCOPE)
endif()
