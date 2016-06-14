cmake_minimum_required(VERSION 2.8)

# Version
set(MYCROFT_MAJOR_VERSION 0)
set(MYCROFT_MINOR_VERSION 1)

# Project name
set(PROJECT_NAME mycroft_core-${MYCROFT_MAJOR_VERSION}.${MYCROFT_MINOR_VERSION})
project(${PROJECT_NAME} C)
set(MYCROFT_VERSION "${PROJECT_NAME}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Source files
set(SRC
    main.c
)

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
    "config.h.in"
    "config.h"
)

# Find capstone lib
include(${CMAKE_SOURCE_DIR}/cmake/FindCapstone.cmake)
if (CAPSTONE_FOUND)
    include_directories(${CAPSTONE_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${CAPSTONE_LIBRARIES})
endif (CAPSTONE_FOUND)

# Find sqlite3 lib
include(${CMAKE_SOURCE_DIR}/cmake/FindSQLite3.cmake)
if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    target_link_libraries (${PROJECT_NAME} ${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)
