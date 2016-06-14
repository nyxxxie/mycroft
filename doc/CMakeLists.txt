include(FindDoxygen)
include(DoxygenTarget)

option(BUILD_DOCS "Build documentation during main build." OFF)
mark_as_advanced(BUILD_DOCS)

if (NOT DOXYGEN_DOT_FOUND)
    message(WARNING "Failed to find Graphviz, Doxygen won't be able to create graphs.")

endif (NOT DOXYGEN_DOT_FOUND)

if (DOXYGEN_FOUND)
    PrepareDocTarget()
endif (DOXYGEN_FOUND)
