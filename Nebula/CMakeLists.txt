cmake_minimum_required(VERSION 3.22)
project(Nebula)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

set(BUILD_LIBRARIES_RELEASE ON)
include(cmake/setup-libraries.cmake)

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/Nebula/include)
add_compile_definitions(NB_BUILD_DLL)

set(NEBULA_SOURCE_FILES
        src/core/Application.cpp
        src/core/Logging.cpp
        src/core/LayerStack.cpp
        src/events/EventManager.cpp
        src/debug/ImGuiLayer.cpp
        src/platform/DetectPlatform.cpp
        src/platform/Windows/WindowsInput.cpp
        src/platform/Windows/WindowsWindow.cpp
        src/platform/OpenGL/OpenGLContext.cpp
        src/memory/MemoryChunk.cpp
        src/memory/MemoryManager.cpp
        src/memory/Allocators.cpp
)

add_library(nebula SHARED ${OPENGL_SOURCE_FILES} ${NEBULA_SOURCE_FILES} ${IMGUI_SOURCE_FILES})
target_link_libraries(nebula ${SPD_LOG} glfw ${YAML_CPP})
target_precompile_headers(nebula PRIVATE include/nebula_pch.h)
add_custom_command(TARGET nebula POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/libnebula.dll ${CMAKE_SOURCE_DIR}/bin/Sandbox/${CMAKE_BUILD_TYPE})
