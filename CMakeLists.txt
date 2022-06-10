cmake_minimum_required(VERSION 3.19)
project(Nebula)

option(BUILD_ENGINE "Enable building Nebula Engine" ON)
option(BUILD_SANDBOX "Enable building test sandbox" ON)

if(BUILD_ENGINE OR BUILD_SANDBOX)
    add_subdirectory(Nebula)
endif()
if(BUILD_SANDBOX)
    add_subdirectory(sandbox)
    set(VS_STARTUP_PROJECT sandbox)

    add_dependencies(sandbox nebula)
    target_link_libraries(sandbox nebula)
endif()
