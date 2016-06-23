# location of header files
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/googletest
    ${CMAKE_SOURCE_DIR}/third_party/googletest/include
)

add_subdirectory(core)
