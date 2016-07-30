cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CORE_MAJOR_VERSION 0)
set(MYCROFT_CORE_MINOR_VERSION 1)

# Use C99 standard
if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_C_FLAGS "--std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
else ()
    set (CMAKE_C_STANDARD 99)
endif ()

# Project name
set(PROJECT_NAME mcore)
project(${PROJECT_NAME})
set(MYCROFT_VERSION "${PROJECT_NAME}")

include_directories(
    ${SQLITE3_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${CAPSTONE_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

# Source files
set(SRC
    core.c
    cfg.c
    file.c
    hook.c
    plugin.c
    config_parse.c
    database.c
    membuf.c
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

# Link essential libraries
target_link_libraries(${PROJECT_NAME}
    ${SQLITE3_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${CAPSTONE_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
