cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CLI_MAJOR_VERSION 0)
set(MYCROFT_CLI_MINOR_VERSION 1)

# Project name
set(PROJECT_NAME mycroft-cli)
project(${PROJECT_NAME})
set(MYCROFT_VERSION "${PROJECT_NAME}")

# Source files
set(SRC
    main.c
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
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} mcore)
