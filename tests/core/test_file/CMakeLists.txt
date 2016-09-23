cmake_minimum_required(VERSION 2.8.11)

# Find required libs
find_package(OpenSSL REQUIRED)

# Create a variable that tells us where our resources are
set(TEST_FILE_RES ${ CMAKE_CURRENT_SOURCE_DIR}/res)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    "config.h.in"
    "config.h"
)

# File test
set(TEST_NAME test_core_file)
add_executable(${TEST_NAME} main.cpp file.cpp)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(${TEST_NAME} mcore gtest ${OPENSSL_LIBRARIES})
add_dependencies(${TEST_BUILD_TARGET} ${TEST_NAME})
add_test(${TEST_NAME} ${TEST_NAME})

# Copy test files over to build directory every time make is run
add_custom_target(${TEST_NAME}_copy_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)

add_dependencies(${TEST_NAME} ${TEST_NAME}_copy_files)
