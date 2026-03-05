cmake_minimum_required(VERSION 3.25.1)

project(third_party VERSION 0.9)

block()
	set(TMP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/msdfgen)
	set(TMP_BINARY_DIR ${PROJECT_BINARY_DIR}/msdfgen)
	if(NOT EXISTS ${TMP_SOURCE_DIR}/CMakeLists.txt)
		execute_process(
			COMMAND git submodule update
				--init third_party/msdfgen
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(COMMAND git checkout v1.13
			WORKING_DIRECTORY ${TMP_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/configure.done)
		execute_process(
			COMMAND cmake
				-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
				-D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
				-D CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
				-D MSDFGEN_CORE_ONLY=ON
				-D MSDFGEN_BUILD_STANDALONE=OFF
				-D MSDFGEN_USE_VCPKG=OFF
				-D MSDFGEN_USE_SKIA=OFF
				-D MSDFGEN_DISABLE_SVG=ON
				-D MSDFGEN_DISABLE_PNG=ON
				-D MSDFGEN_INSTALL=ON
				-B . -S ${TMP_SOURCE_DIR}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch configure.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/build.done)
		execute_process(
			COMMAND cmake
				--build . --config ${CMAKE_BUILD_TYPE}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch build.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/install.done)
		execute_process(
			COMMAND cmake
				--install . --config ${CMAKE_BUILD_TYPE}
				--prefix ${CMAKE_INSTALL_PREFIX}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch install.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
endblock()

block()
	set(TMP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/oklab)
	set(TMP_BINARY_DIR ${PROJECT_BINARY_DIR}/oklab)
	if(NOT EXISTS ${TMP_SOURCE_DIR}/CMakeLists.txt)
		execute_process(
			COMMAND git submodule update
				--init third_party/oklab
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(COMMAND git checkout 0.9.0
			WORKING_DIRECTORY ${TMP_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/configure.done)
		execute_process(
			COMMAND cmake
				-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
				-D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
				-D CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
				-B . -S ${TMP_SOURCE_DIR}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch configure.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/build.done)
		execute_process(
			COMMAND cmake
				--build . --config ${CMAKE_BUILD_TYPE}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch build.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/install.done)
		execute_process(
			COMMAND cmake
				--install . --config ${CMAKE_BUILD_TYPE}
				--prefix ${CMAKE_INSTALL_PREFIX}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch install.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
endblock()

block()
	set(TMP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vbvmr)
	set(TMP_BINARY_DIR ${PROJECT_BINARY_DIR}/vbvmr)
	if(NOT EXISTS ${TMP_SOURCE_DIR}/CMakeLists.txt)
		execute_process(
			COMMAND git submodule update
				--init third_party/vbvmr
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(COMMAND git checkout 0.9.0
			WORKING_DIRECTORY ${TMP_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/configure.done)
		execute_process(
			COMMAND cmake
				-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
				-D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
				-D CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
				-B . -S ${TMP_SOURCE_DIR}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch configure.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/build.done)
		execute_process(
			COMMAND cmake
				--build . --config ${CMAKE_BUILD_TYPE}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch build.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/install.done)
		execute_process(
			COMMAND cmake
				--install . --config ${CMAKE_BUILD_TYPE}
				--prefix ${CMAKE_INSTALL_PREFIX}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch install.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
endblock()

block()
	set(TMP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/windows)
	set(TMP_BINARY_DIR ${PROJECT_BINARY_DIR}/windows)
	if(NOT EXISTS ${TMP_SOURCE_DIR}/CMakeLists.txt)
		execute_process(
			COMMAND git submodule update
				--init third_party/windows
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(COMMAND git checkout 0.9.0
			WORKING_DIRECTORY ${TMP_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/configure.done)
		execute_process(
			COMMAND cmake
				-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
				-D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
				-D CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
				-B . -S ${TMP_SOURCE_DIR}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch configure.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/build.done)
		execute_process(
			COMMAND cmake
				--build . --config ${CMAKE_BUILD_TYPE}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch build.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/install.done)
		execute_process(
			COMMAND cmake
				--install . --config ${CMAKE_BUILD_TYPE}
				--prefix ${CMAKE_INSTALL_PREFIX}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch install.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
endblock()

block()
	set(TMP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/wstd)
	set(TMP_BINARY_DIR ${PROJECT_BINARY_DIR}/wstd)
	if(NOT EXISTS ${TMP_SOURCE_DIR}/CMakeLists.txt)
		execute_process(
			COMMAND git submodule update
				--init third_party/wstd
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(COMMAND git checkout 0.9.0
			WORKING_DIRECTORY ${TMP_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/configure.done)
		execute_process(
			COMMAND cmake
				-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
				-D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
				-D CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
				-B . -S ${TMP_SOURCE_DIR}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch configure.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/build.done)
		execute_process(
			COMMAND cmake
				--build . --config ${CMAKE_BUILD_TYPE}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch build.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/install.done)
		execute_process(
			COMMAND cmake
				--install . --config ${CMAKE_BUILD_TYPE}
				--prefix ${CMAKE_INSTALL_PREFIX}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch install.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
endblock()

block()
	set(TMP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/wui)
	set(TMP_BINARY_DIR ${PROJECT_BINARY_DIR}/wui)
	if(NOT EXISTS ${TMP_SOURCE_DIR}/CMakeLists.txt)
		execute_process(
			COMMAND git submodule update
				--init third_party/wui
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(COMMAND git checkout 0.9.0
			WORKING_DIRECTORY ${TMP_SOURCE_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/configure.done)
		execute_process(
			COMMAND cmake
				-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
				-D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
				-D CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
				-D WUI_GRAPHICS=${WUI_GRAPHICS}
				-D WUI_PALETTE=${WUI_PALETTE}
				-D WUI_LOADER=${WUI_LOADER}
				-D WUI_INSTALL=ON
				-B . -S ${TMP_SOURCE_DIR}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch configure.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/build.done)
		execute_process(
			COMMAND cmake
				--build . --config ${CMAKE_BUILD_TYPE}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch build.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
	if(NOT EXISTS ${TMP_BINARY_DIR}/install.done)
		execute_process(
			COMMAND cmake
				--install . --config ${CMAKE_BUILD_TYPE}
				--prefix ${CMAKE_INSTALL_PREFIX}
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
		execute_process(
			COMMAND cmake
				-E touch install.done
			WORKING_DIRECTORY ${TMP_BINARY_DIR}
			COMMAND_ERROR_IS_FATAL ANY)
	endif()
endblock()
