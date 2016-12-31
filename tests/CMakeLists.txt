# Location of header files
include_directories(
    ${MYCROFT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/third_party/googletest
    ${CMAKE_SOURCE_DIR}/third_party/googletest/include
)

# Make cmake build stuff necessary to run tests
set(TEST_BUILD_TARGET build_and_test)
add_custom_target(${TEST_BUILD_TARGET} ${CMAKE_CTEST_COMMAND} -V)

add_subdirectory(core)
add_subdirectory(template)
