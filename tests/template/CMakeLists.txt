include_directories(
    ${MYCROFT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib/mycroft_core
)

add_subdirectory(typesystem)
add_subdirectory(template)
