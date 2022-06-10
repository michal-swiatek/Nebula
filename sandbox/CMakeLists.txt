cmake_minimum_required(VERSION 3.19)
project(sandbox)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/)

include_directories(${CMAKE_SOURCE_DIR}/Nebula/src)

set(SANDBOX_HEADER_FILES ${CMAKE_SOURCE_DIR}/Nebula/src/Nebula.h)

add_executable(sandbox app.cpp ${SANDBOX_HEADER_FILES})
