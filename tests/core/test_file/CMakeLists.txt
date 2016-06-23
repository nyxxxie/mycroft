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

# Add test to ctest target
add_test(
    NAME ${TEST_CORE_FILE}
    COMMAND ${TEST_CORE_FILE}
)
