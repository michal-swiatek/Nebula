cmake_minimum_required(VERSION 3.19)
project(nebula_engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/)

include_directories(src)

add_compile_definitions(BUILD_NEBULA_DLL)

set(NEBULA_SOURCE_FILES src/Engine/Application.cpp)

set(NEBULA_HEADER_FILES src/Engine/EntryPoint.h
                        src/Engine/NebulaAPI.h
                        src/Engine/Application.h)

add_library(nebula SHARED ${NEBULA_SOURCE_FILES} ${NEBULA_HEADER_FILES})
