cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CORE_MAJOR_VERSION 0)
set(MYCROFT_CORE_MINOR_VERSION 1)

# Use C99 standard
if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_FLAGS "--std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
else ()
    set(CMAKE_C_STANDARD 99)
endif ()

project(${MYCROFT_CORE_NAME})
set(MYCROFT_CORE_VERSION "${MYCROFT_CORE_NAME}")

include_directories(
    ${SQLITE3_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${CAPSTONE_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

# Source files
set(MYCROFT_CORE_SRC
    context.c
    project.c
    file.c
    hook.c
    plugin.c
    script.c
    database.c
    membuf.c
    binds/context.c
    binds/file.c
    template/template.c
    template/template_parser.c
)

# Save me from manually including the files ;)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify the library to be built
add_library(${MYCROFT_CORE_NAME} STATIC ${MYCROFT_CORE_SRC})

# Link essential libraries
target_link_libraries(${MYCROFT_CORE_NAME}
    ${SQLITE3_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${CAPSTONE_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

# Install library
install(TARGETS ${MYCROFT_CORE_NAME} ARCHIVE DESTINATION lib)