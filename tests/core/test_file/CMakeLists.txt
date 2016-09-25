cmake_minimum_required(VERSION 2.8.11)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

# Set test name
set(TEST_FILE_NAME test_file)

# Include external headers
include_directories(${OPENSSL_INCLUDE_DIRS})

# Create target
add_executable(${TEST_FILE_NAME}
    main.cpp
    file.cpp
)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_FILE_NAME})

# Link libs
target_link_libraries(${TEST_FILE_NAME}
    ${MYCROFT_CORE_NAME}
    gtest
    ${OPENSSL_LIBRARIES}
)

# Create a variable that tells us where our resources are
set(TEST_FILE_SRC_RES ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(TEST_FILE_BIN_RES ${CMAKE_CURRENT_BINARY_DIR}/res)

# Copy resources every time make is run
add_custom_target(${TEST_FILE_NAME}_copy_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${TEST_FILE_SRC_RES}/res ${TEST_FILE_BIN_RES}/res
)
add_dependencies(${TEST_FILE_NAME} ${TEST_FILE_NAME}_copy_files)

# Make CTest recognise this target as a test
add_test(${TEST_FILE_NAME} ${TEST_FILE_NAME})
