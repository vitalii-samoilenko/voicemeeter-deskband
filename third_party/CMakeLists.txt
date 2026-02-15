cmake_minimum_required(VERSION 3.25.1)

project(third_party VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

block()
  set(ABSEIL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/abseil)
  set(ABSEIL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/abseil)
  if(NOT EXISTS ${ABSEIL_SOURCE_DIR}/CMakeLists.txt)
    execute_process(COMMAND git submodule update
        --init third_party/abseil
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND git checkout 20250512.1
      WORKING_DIRECTORY ${ABSEIL_SOURCE_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  if(NOT EXISTS ${ABSEIL_BINARY_DIR}/configure.done)
    execute_process(COMMAND cmake
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DABSL_ENABLE_INSTALL=ON
        -DABSL_MSVC_STATIC_RUNTIME=ON
        -B ${ABSEIL_BINARY_DIR} -S .
      WORKING_DIRECTORY ${ABSEIL_SOURCE_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND cmake
        -E touch configure.done
      WORKING_DIRECTORY ${ABSEIL_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  if(NOT EXISTS ${ABSEIL_BINARY_DIR}/build.done)
    execute_process(COMMAND cmake
        --build . --config ${CMAKE_BUILD_TYPE}
      WORKING_DIRECTORY ${ABSEIL_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND cmake
        -E touch build.done
      WORKING_DIRECTORY ${ABSEIL_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  if(NOT EXISTS ${ABSEIL_BINARY_DIR}/install.done)
    execute_process(COMMAND cmake
        --install . --config ${CMAKE_BUILD_TYPE} --prefix ${CMAKE_INSTALL_PREFIX}
      WORKING_DIRECTORY ${ABSEIL_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND cmake
        -E touch install.done
      WORKING_DIRECTORY ${ABSEIL_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
endblock()

block()
  set(RE2_SOURCE_DIR ${PROJECT_SOURCE_DIR}/re2)
  set(RE2_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/re2)
  if(NOT EXISTS ${RE2_SOURCE_DIR}/CMakeLists.txt)
    execute_process(COMMAND git submodule update
        --init third_party/re2
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND git checkout 2025-07-17
      WORKING_DIRECTORY ${RE2_SOURCE_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  if(NOT EXISTS ${RE2_BINARY_DIR}/configure.done)
    execute_process(COMMAND cmake
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_POLICY_DEFAULT_CMP0091=NEW
        -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
        -B ${RE2_BINARY_DIR} -S .
      WORKING_DIRECTORY ${RE2_SOURCE_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND cmake
        -E touch configure.done
      WORKING_DIRECTORY ${RE2_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  if(NOT EXISTS ${RE2_BINARY_DIR}/build.done)
    execute_process(COMMAND cmake
        --build . --config ${CMAKE_BUILD_TYPE}
      WORKING_DIRECTORY ${RE2_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND cmake
        -E touch build.done
      WORKING_DIRECTORY ${RE2_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  if(NOT EXISTS ${RE2_BINARY_DIR}/install.done)
    execute_process(COMMAND cmake
        --install . --config ${CMAKE_BUILD_TYPE} --prefix ${CMAKE_INSTALL_PREFIX}
      WORKING_DIRECTORY ${RE2_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
    execute_process(COMMAND cmake
        -E touch install.done
      WORKING_DIRECTORY ${RE2_BINARY_DIR}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
endblock()
