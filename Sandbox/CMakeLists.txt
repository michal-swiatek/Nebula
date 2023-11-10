cmake_minimum_required(VERSION 3.22)
project(Sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE})

include_directories(${CMAKE_SOURCE_DIR}/Nebula/include)
include_directories(${CMAKE_SOURCE_DIR}/Nebula/3rd-party/glm)
include_directories(${CMAKE_SOURCE_DIR}/Nebula/3rd-party/spdlog/include)

add_executable(sandbox main.cpp)
target_link_libraries(sandbox nebula)
