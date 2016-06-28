set(MAIN_PROJECT_NAME mycroft)

set(CMAKE_BUILD_TYPE RELEASE)

# Set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# TODO: ADD "INSTALL" in these directory names
# setting MYCROFT_DATA_PATH
if(DEFINED MYCROFT_DATA_PATH)
    set(MYCROFT_DATA_PATH ${MYCROFT_DATA_PATH} CACHE PATH "mycroft share directory")
else()
    set(MYCROFT_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_AWE_NAME} CACHE PATH "mycroft share directory")
endif()

# setting MYCROFT_DOC_PATH
if(DEFINED MYCROFT_DOC_PATH)
    set(MYCROFT_DOC_PATH ${MYCROFT_DOC_PATH} CACHE PATH "mycroft docs directory")
else()
    set(MYCROFT_DOC_PATH ${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_AWE_NAME} CACHE PATH "mycroft docs directory")
endif()

# set man path
if(DEFINED MYCROFT_MAN_PATH)
   set(MYCROFT_MAN_PATH ${MYCROFT_MAN_PATH} CACHE PATH "mycroft manpage directory")
else()
   set(MYCROFT_MAN_PATH ${CMAKE_INSTALL_PREFIX}/share/man CACHE PATH "mycroft manpage directory")
endif()

# Set project directory locations
set(MYCROFT_CORE_DIR ${CMAKE_SOURCE_DIR}/src/core)
set(MYCROFT_CLIENT_DIR ${CMAKE_SOURCE_DIR}/src/client)
set(MYCROFT_TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(MYCROFT_TEST_RESOURCES_DIR ${CMAKE_SOURCE_DIR}/tests/resources) # TODO: add some code that copies these into the build directory by default (or pass them somehow to tests)
