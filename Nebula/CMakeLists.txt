cmake_minimum_required(VERSION 3.22)
project(Nebula)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

include(cmake/setup-libraries.cmake)

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/Nebula/include)
add_compile_definitions(NB_BUILD_DLL)

add_library(nebula SHARED src/core/Application.cpp)
add_custom_command(TARGET nebula POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/libnebula.dll ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE})
