cmake_minimum_required(VERSION 2.8)

# Project name
set(PROJECT_NAME mycroftd)
project(${PROJECT_NAME} C)

# Version
set(MYCROFTD_MAJOR_VERSION 0)
set(MYCROFTD_MINOR_VERSION 1)
set(MYCROFTD_VERSION "${PROJECT_NAME} ${MYCROFTD_MAJOR_VERSION}.${MYCROFTD_MINOR_VERSION}")

# Establish directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

# Source files
set(SRC
    ${SOURCE_DIR}/main.c)

# Set the c compiler
set(CMAKE_C_COMPILER gcc)

# CFLAGS
set(CMAKE_C_FLAGS
    -Wall)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${SOURCE_DIR}/config.h.in"
    "${SOURCE_DIR}/config.h"
)

# Specify the exe to be built
add_executable(${PROJECT_NAME} ${SRC})
