include_directories(
    ${MYCROFT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/mycroft_core
)

add_subdirectory(file)
add_subdirectory(project)
add_subdirectory(context)
add_subdirectory(plugin)
