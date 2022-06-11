cmake_minimum_required(VERSION 3.19)
project(nebula_engine)

# Check toolchain
if (MSVC)
    set(TOOLCHAIN msvc)
else()
    set(TOOLCHAIN mingw)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin/${TOOLCHAIN}/$<CONFIG>/${PROJECT_NAME}/)

include_directories(src)

add_compile_definitions(BUILD_NEBULA_DLL)

#-----------------------------------------------------------------------------------------------------------------------
#-----------------------------------------------BUILD DEPENDENCIES------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------

macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

#
# STATIC LIBRARY - spdlog
#
SET_OPTION(SPDLOG_ENABLE_PCH ON)
add_subdirectory(third-party/spdlog)
include_directories(third-party/spdlog/include)

#-----------------------------------------------------------------------------------------------------------------------
#-----------------------------------------------BUILD PROJECT-----------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------

set(NEBULA_SOURCE_FILES src/Engine/Application.cpp
                        src/Engine/Logger.cpp)

set(NEBULA_HEADER_FILES src/Engine/EntryPoint.h
                        src/Engine/NebulaAPI.h
                        src/Engine/Application.h
                        src/Engine/Logger.h)

add_library(nebula SHARED ${NEBULA_SOURCE_FILES} ${NEBULA_HEADER_FILES})

target_link_libraries(nebula PUBLIC spdlog::spdlog)
target_compile_definitions(nebula PUBLIC SPDLOG_COMPILED_LIB)
target_compile_definitions(nebula PUBLIC RELEASE_LOGS)  # TODO: Remove in final build

# Copy dll after build
if (${TOOLCHAIN} MATCHES mingw)
    add_custom_command(TARGET nebula POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE_DIR:nebula>/libnebula.dll"
            $<TARGET_FILE_DIR:sandbox>)
else()
    add_custom_command(TARGET nebula POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE_DIR:nebula>/nebula.dll"
            $<TARGET_FILE_DIR:sandbox>)
endif()
