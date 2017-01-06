cmake_minimum_required(VERSION 2.8.11)

# Save me from manually including the files ;)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

add_executable(${MYCROFT_TOOL_PYTESBENCH_NAME} main.c)
add_dependencies(${MYCROFT_TOOL_PYTESBENCH_NAME} ${MYCROFT_CORE_NAME})
