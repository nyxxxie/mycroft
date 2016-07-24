include(FindDoxygen)

option(BUILD_DOCS "Build documentation during main build." OFF)
mark_as_advanced(BUILD_DOCS)

if (DOXYGEN_FOUND)
    if (NOT DOXYGEN_DOT_FOUND)
        message(WARNING "Failed to find Graphviz, Doxygen won't be able to create graphs.")
    endif (NOT DOXYGEN_DOT_FOUND)

    configure_file (
            ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.in
            ${CMAKE_CURRENT_BINARY_DIR}/doxygen @ONLY
    )

    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
else ()
    message(WARNING "Failed to find doxygen, can't generate api docs.")
endif (DOXYGEN_FOUND)
