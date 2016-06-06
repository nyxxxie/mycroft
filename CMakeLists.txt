cmake_minimum_required(VERSION 2.8)

# Version
set(MYCROFT_MAJOR_VERSION 0)
set(MYCROFT_MINOR_VERSION 1)

# Project name
set(PROJECT_NAME mycroft_core-${MYCROFT_MAJOR_VERSION}.${MYCROFT_MINOR_VERSION})
project(${PROJECT_NAME} C)
set(MYCROFT_VERSION "${PROJECT_NAME}")

# Establish directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

# Source files
set(SRC
    ${SOURCE_DIR}/main.c)

# Specify the exe to be built
add_executable(${PROJECT_NAME} ${SRC})

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
# Find capstone lib
include(${CMAKE_SOURCE_DIR}/cmake/FindCapstone.cmake)
if (CAPSTONE_FOUND)
    message(STATUS "FOUND THE GOODS")
    include_directories(${CAPSTONE_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${CAPSTONE_LIBRARIES})
endif (CAPSTONE_FOUND)
