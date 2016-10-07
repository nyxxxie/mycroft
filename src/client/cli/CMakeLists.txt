cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CLIENT_CLI_MAJOR_VERSION 0)
set(MYCROFT_CLIENT_CLI_MINOR_VERSION 1)

# Project name
project(${MYCROFT_CLIENT_CLI_NAME})

# Source files
set(MYCROFT_CLIENT_CLI_SRC
    main.c
)

# Save me from manually including the files ;)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

# Specify the library to be built
add_executable(${MYCROFT_CLIENT_CLI_NAME} ${MYCROFT_CLIENT_CLI_SRC})
add_dependencies(${MYCROFT_CLIENT_CLI_NAME} ${MYCROFT_CORE_NAME})

# Link libraries
target_link_libraries(${MYCROFT_CLIENT_CLI_NAME} ${MYCROFT_CORE_NAME})
