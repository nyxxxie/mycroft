cmake_minimum_required(VERSION 2.8.11)
# Move test materials to build directory

# File test
set(TEST_CORE_FILE test_core_file)
project(${TEST_CORE_FILE})
set(TEST_CORE_FILE_SRC
    main.cpp
    file.cpp
)

add_executable(${TEST_CORE_FILE} ${TEST_CORE_FILE_SRC})

target_link_libraries(${TEST_CORE_FILE} mcore gtest)

# Copy test files over to build directory every time make is run
add_custom_target(copy_test_files_core
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)

add_dependencies(${TEST_CORE_FILE} copy_test_files_core)

#message(STATUS ${CMAKE_CURRENT_LIST_DIR})

# Add test to ctest target
add_test(
    NAME ${TEST_CORE_FILE}
    COMMAND ${TEST_CORE_FILE}
)
