cmake_minimum_required(VERSION 2.8.11)

set(TEST_NAME test_core_core)
add_executable(${TEST_NAME} main.cpp core.cpp)
target_link_libraries(${TEST_NAME} mcore gtest)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_NAME})
add_test(${TEST_NAME} ${TEST_NAME})

# Copy test files over to build directory every time make is run
add_custom_target(${TEST_NAME}_copy_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)

add_dependencies(${TEST_NAME} ${TEST_NAME}_copy_files)
