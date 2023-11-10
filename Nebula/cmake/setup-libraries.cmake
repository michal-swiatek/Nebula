set(OPENGL_MAJOR_VERSION 4)
set(OPENGL_MINOR_VERSION 6)
configure_file(include/OpenGLConfiguration.h.in include/OpenGLConfiguration.h @ONLY)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party)

set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
if (${BUILD_LIBRARIES_RELEASE})
    set(CMAKE_BUILD_TYPE "Release")
endif()
message("Building 3rd-party libraries in ${CMAKE_BUILD_TYPE} mode.")

# spdlog
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "Use std::format")
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "Use precompiled headers")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/spdlog)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/spdlog/include)
set(SPD_LOG ${spdlog})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glfw/include)

# glad
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glad/include)
set(OPENGL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glad/src/glad.cpp)

# glm
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/glm)

# imgui
include_directories(3rd-party/imgui)
set(IMGUI_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui_demo.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui_draw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui_tables.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui_widgets.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/backends/imgui_impl_glfw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/backends/imgui_impl_opengl3.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/misc/cpp/imgui_stdlib.cpp")

set(IMGUI_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imconfig.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imgui_internal.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imstb_rectpack.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imstb_textedit.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/imstb_truetype.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/backends/imgui_impl_glfw.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/backends/imgui_impl_opengl3.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui/misc/cpp/imgui_stdlib.h")

# Restore project build type
set(CMAKE_BUILD_TYPE ${BUILD_TYPE})
