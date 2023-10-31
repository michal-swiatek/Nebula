cmake_minimum_required(VERSION 3.22)
project(Nebula)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Nebula/bin/${CMAKE_BUILD_TYPE})

include(cmake/setup-libraries.cmake)

set(OPENGL_MAJOR_VERSION 4)
set(OPENGL_MINOR_VERSION 6)

configure_file(include/OpenGLConfiguration.h.in include/OpenGLConfiguration.h.in @ONLY)

