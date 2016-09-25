cmake_minimum_required(VERSION 2.8.11)

# Set test name
set(TEST_CORE_NAME test_core)

# Create target
add_executable(${TEST_CORE_NAME}
    main.cpp
    core.cpp
)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_CORE_NAME})

# Link libs
target_link_libraries(${TEST_CORE_NAME}
    ${MYCROFT_CORE_NAME}
    gtest
)

# Copy resources every time make is run
add_custom_target(${TEST_CORE_NAME}_copy_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)
add_dependencies(${TEST_CORE_NAME} ${TEST_CORE_NAME}_copy_files)

# Make CTest recognise this target as a test
add_test(${TEST_CORE_NAME} ${TEST_CORE_NAME})
