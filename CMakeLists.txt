cmake_minimum_required(VERSION 3.25.1)

project(Voicemeeter VERSION 0.9)


set(LOADER Windows)# Choose resource manager: Windows
set(PALETTE Oklab)# Choose color blender: Oklab
set(ATLAS Cherry)# Choose texture set: Cherry
set(GRAPHICS D3D12)# Choose rendering engine: D3D12
set(CONTROLS Cherry)# Choose widget set: Cherry

if(MSVC)
  add_compile_options(/bigobj)
  if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
  else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
  endif()
  add_compile_options(/GR-)
elseif(LINUX)
  set(CMAKE_EXE_LINKER_FLAGS "--static -static-libgcc -static-libstdc++")
endif()

add_subdirectory(src/estd)
add_subdirectory(src/Voicemeeter)
add_subdirectory(src/Voicemeeter.Clients.Remote)
if(ATLAS STREQUAL Cherry)
	add_subdirectory(src/Voicemeeter.UI.Atlas.Cherry)
endif()
if(CONTROLS STREQUAL Cherry)
	add_subdirectory(src/Voicemeeter.Clients.UI.Controls.Cherry)
	add_subdirectory(src/Voicemeeter.UI.Controls.Cherry)
endif()
if(GRAPHICS STREQUAL D3D12)
	add_subdirectory(src/Voicemeeter.Clients.UI.Graphics.D3D12)
	add_subdirectory(src/Voicemeeter.UI.Graphics.D3D12)
endif()
if(LOADER STREQUAL Windows)
	add_subdirectory(src/Voicemeeter.Clients.UI.Loader.Windows)
	add_subdirectory(src/Voicemeeter.UI.Loader.Windows)
endif()
if(PALETTE STREQUAL Oklab)
	add_subdirectory(src/Voicemeeter.Clients.UI.Palette.Oklab)
	add_subdirectory(src/Voicemeeter.UI.Palette.Oklab)
endif()
add_subdirectory(src/Voicemeeter.Clients.UI.Scene)
add_subdirectory(src/Voicemeeter.DeskBandit)
add_subdirectory(src/Voicemeeter.UI)
add_subdirectory(src/Voicemeeter.UI.Layouts)
add_subdirectory(src/wheel)
add_subdirectory(src/Windows)
