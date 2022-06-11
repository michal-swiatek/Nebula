cmake_minimum_required(VERSION 3.19)
project(sandbox)

# Check toolchain
if (MSVC)
    set(TOOLCHAIN msvc)
else()
    set(TOOLCHAIN mingw)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin/${TOOLCHAIN}/$<CONFIG>/${PROJECT_NAME}/)

include_directories(${CMAKE_SOURCE_DIR}/Nebula/src)
include_directories(${CMAKE_SOURCE_DIR}/Nebula/third-party/spdlog/include)

set(SANDBOX_HEADER_FILES ${CMAKE_SOURCE_DIR}/Nebula/src/Nebula.h)

add_executable(sandbox app.cpp ${SANDBOX_HEADER_FILES})
target_compile_definitions(sandbox PUBLIC SPDLOG_COMPILED_LIB)
