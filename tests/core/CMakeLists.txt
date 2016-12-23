include_directories(
    ${MYCROFT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/mycroft_core
)

add_subdirectory(test_file)
add_subdirectory(test_project)
add_subdirectory(test_context)
add_subdirectory(test_plugin)
add_subdirectory(test_template)
