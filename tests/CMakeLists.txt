# location of header files
include_directories(
    ${MYCROFT_CORE_SRC_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/googletest
    ${CMAKE_SOURCE_DIR}/third_party/googletest/include
)

# TODO: use google cmake macros instead

# TODO: Make a global testing target, and use this file as a template for
#       tests in core, client/cli, etc
add_executable(unit_tests
    core/main.cpp
    core/test.cpp)

target_link_libraries(unit_tests gtest)

# TODO: CTest doesn't show gtest's output, just that a test failed.  Make a
#       custom target?

# This adds a test to the CMake test target
add_test(
    NAME unit_tests
    COMMAND unit_tests
)
