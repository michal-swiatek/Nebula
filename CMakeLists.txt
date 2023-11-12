cmake_minimum_required(VERSION 3.22)
project(NebulaEngine)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Ofast -flto=thin)
endif ()

#   Add sub-projects
add_subdirectory(Nebula)
add_subdirectory(Sandbox)
