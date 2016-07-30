cmake_minimum_required(VERSION 2.8.11)

# File test
set(TEST_NAME test_core_plugin)
add_executable(${TEST_NAME} main.cpp tests.cpp)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(${TEST_NAME} mcore gtest)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_NAME})
add_test(${TEST_NAME} ${TEST_NAME})
