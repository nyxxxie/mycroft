include_directories(
    ${MYCROFT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/${MYCROFT_CORE_NAME}
)

add_subdirectory(test_file)
add_subdirectory(test_project)
add_subdirectory(test_database)
add_subdirectory(test_plugin)
add_subdirectory(test_template)
