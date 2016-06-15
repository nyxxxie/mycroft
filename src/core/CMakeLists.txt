cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CORE_MAJOR_VERSION 0)
set(MYCROFT_CORE_MINOR_VERSION 1)

# Project name
set(PROJECT_NAME mcore)
project(${PROJECT_NAME})
set(MYCROFT_VERSION "${PROJECT_NAME}")

# Source files
set(SRC
    template.c
)

# Save me from manually including the files ;)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    "config.h.in"
    "config.h"
)

# Specify the library to be built
add_library(${PROJECT_NAME} STATIC ${SRC})

# Find capstone lib
include(${CMAKE_SOURCE_DIR}/cmake/FindCapstone.cmake)
if (CAPSTONE_FOUND)
    include_directories(${CAPSTONE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${CAPSTONE_LIBRARIES})
endif (CAPSTONE_FOUND)

# Find sqlite3 lib
include(${CMAKE_SOURCE_DIR}/cmake/FindSQLite3.cmake)
if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)
