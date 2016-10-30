cmake_minimum_required(VERSION 2.8.11)

# Set test name
set(TEST_PROJECT_NAME test_project)

# Create target
add_executable(${TEST_PROJECT_NAME}
    main.cpp
    project.cpp
)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_PROJECT_NAME})

# Link libs
target_link_libraries(${TEST_PROJECT_NAME}
    ${MYCROFT_CORE_NAME}
    gtest
)

# Create a variable that tells us where our resources are
set(TEST_PROJECT_SRC_RES ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(TEST_PROJECT_BIN_RES ${CMAKE_CURRENT_BINARY_DIR}/res)

# Copy resources every time make is run
add_custom_target(${TEST_PROJECT_NAME}_copy_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${TEST_PROJECT_SRC_RES} ${TEST_PROJECT_BIN_RES}
)
add_dependencies(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME}_copy_files)

# Make CTest recognise this target as a test
add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})
