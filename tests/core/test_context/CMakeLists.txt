cmake_minimum_required(VERSION 2.8.11)

# Set test name
set(TEST_CONTEXT_NAME test_context)

# Create target
add_executable(${TEST_CONTEXT_NAME}
    main.cpp
    context.cpp
)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_CONTEXT_NAME})

# Link libs
target_link_libraries(${TEST_CONTEXT_NAME}
    ${MYCROFT_CORE_NAME}
    gtest
)

# Make CTest recognise this target as a test
add_test(${TEST_CONTEXT_NAME} ${TEST_CONTEXT_NAME})
