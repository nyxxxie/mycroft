# Define project name
set(MYCROFT_NAME mycroft)

# Define target names.  THESE MUST MATCH UP WITH DIRECTORIES.
set(MYCROFT_CLIENT_QT_NAME q${MYCROFT_NAME})
set(MYCROFT_CORE_NAME ${MYCROFT_NAME}_core)

# Define finished binary file names
set(MYCROFT_CLIENT_BIN ${MYCROFT_CLIENT_NAME})
set(MYCROFT_CLI_BIN ${MYCROFT_CLI_NAME})
set(MYCROFT_CORE_BIN ${MYCROFT_NAME}${MYCROFT_CORE_NAME})

# Define common project include directories
set(MYCROFT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

# Set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Local base directory.  Used to store user-specific data
if (DEFINED MYCROFT_LOCAL_BASE)
    set(MYCROFT_LOCAL_BASE ${MYCROFT_LOCAL_BASE} CACHE PATH "mycroft local directory")
else ()
    set(MYCROFT_LOCAL_BASE $ENV{HOME}/.${MAIN_PROJECT_NAME} CACHE PATH "mycroft local directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_DATA_PATH)
    set(MYCROFT_DATA_PATH ${MYCROFT_DATA_PATH} CACHE PATH "mycroft data directory")
else ()
    set(MYCROFT_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/${MYCROFT_NAME} CACHE PATH "mycroft sys data directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_DOC_PATH)
    set(MYCROFT_DOC_PATH ${MYCROFT_DOC_PATH} CACHE PATH "mycroft doc directory")
else ()
    set(MYCROFT_DOC_PATH ${CMAKE_INSTALL_PREFIX}/share/doc/${MYCROFT_NAME} CACHE PATH "mycroft sys doc directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_LIB_PATH)
    set(MYCROFT_LIB_PATH ${MYCROFT_LIB_PATH} CACHE PATH "mycroft lib directory")
else ()
    set(MYCROFT_LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "mycroft lib directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_BIN_PATH)
    set(MYCROFT_BIN_PATH ${MYCROFT_BIN_PATH} CACHE PATH "mycroft bin directory")
else ()
    set(MYCROFT_BIN_PATH ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "mycroft bin directory")
endif ()

# Set some local directories and files
set(MYCROFT_INSTALL_PLUGIN_PATH   ${MYCROFT_DATA_PATH}/plugins)
set(MYCROFT_INSTALL_SCRIPT_PATH   ${MYCROFT_DATA_PATH}/scripts)
set(MYCROFT_INSTALL_TEMPLATE_PATH ${MYCROFT_DATA_PATH}/templates)
set(MYCROFT_INSTALL_TYPE_PATH     ${MYCROFT_DATA_PATH}/types)
set(MYCROFT_INSTALL_LIB_CORE      ${MYCROFT_LIB_PATH}/core)
set(MYCROFT_INSTALL_CLIENT_QT     ${MYCROFT_LIB_PATH}/bin)

# Set project directory locations
set(MYCROFT_LOCAL_PLUGIN_PATH   ${CMAKE_SOURCE_DIR}/res/plugins)
set(MYCROFT_LOCAL_SCRIPT_PATH   ${CMAKE_SOURCE_DIR}/res/scripts)
set(MYCROFT_LOCAL_TEMPLATE_PATH ${CMAKE_SOURCE_DIR}/res/templates)
set(MYCROFT_LOCAL_TYPES_PATH    ${CMAKE_SOURCE_DIR}/res/types)
set(MYCROFT_LOCAL_LIB_CORE      ${CMAKE_SOURCE_DIR}/core)
set(MYCROFT_LOCAL_CLIENT_QT     ${CMAKE_SOURCE_DIR}/client/qt)
set(MYCROFT_LOCAL_CORE_PATH     ${CMAKE_SOURCE_DIR}/lib/core)
set(MYCROFT_LOCAL_CLIENT_PATH   ${CMAKE_SOURCE_DIR}/src/client)
set(MYCROFT_LOCAL_TEST_PATH     ${CMAKE_SOURCE_DIR}/tests)
