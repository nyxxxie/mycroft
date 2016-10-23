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

# Make CTest recognise this target as a test
add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})
