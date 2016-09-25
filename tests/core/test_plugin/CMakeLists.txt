cmake_minimum_required(VERSION 2.8.11)

# Set test name
set(TEST_PLUGIN_NAME test_core_plugin)

# Include external headers
include_directories(${OPENSSL_INCLUDE_DIRS})

# Create target
add_executable(${TEST_PLUGIN_NAME}
    main.cpp
    tests.cpp
)
add_dependencies(${TEST_BUILD_TARGET} ${TEST_PLUGIN_NAME})

# Link libs
target_link_libraries(${TEST_PLUGIN_NAME}
    ${MYCROFT_CORE_NAME}
    gtest
)

# Make CTest recognise this target as a test
add_test(${TEST_PLUGIN_NAME} ${TEST_PLUGIN_NAME})
