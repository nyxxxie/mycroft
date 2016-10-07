# Define project name
set(MYCROFT_NAME mycroft)

# Define target names.  THESE MUST MATCH UP WITH DIRECTORIES.
set(MYCROFT_CLIENT_QT_NAME ${MYCROFT_NAME}-qt)
set(MYCROFT_CLIENT_CLI_NAME ${MYCROFT_NAME}-cli)
set(MYCROFT_CORE_NAME core)

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

# Sys base directory.  Used to store global data.
if (DEFINED MYCROFT_SYS_BASE)
    set(MYCROFT_SYS_BASE ${MYCROFT_SYS_BASE} CACHE PATH "mycroft sys directory")
else ()
    set(MYCROFT_SYS_BASE ${CMAKE_INSTALL_PREFIX} CACHE PATH "mycroft sys directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_DATA_PATH)
    set(MYCROFT_DATA_PATH ${MYCROFT_DATA_PATH} CACHE PATH "mycroft sys data directory")
else ()
    set(MYCROFT_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_AWE_NAME} CACHE PATH "mycroft sys data directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_DOC_PATH)
    set(MYCROFT_DOC_PATH ${MYCROFT_DOC_PATH} CACHE PATH "mycroft sys doc directory")
else ()
    set(MYCROFT_DOC_PATH ${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_AWE_NAME} CACHE PATH "mycroft sys doc directory")
endif ()

# Set sys data directory
if (DEFINED MYCROFT_LIB_PATH)
    set(MYCROFT_LIB_PATH ${MYCROFT_LIB_PATH} CACHE PATH "mycroft sys lib directory")
else ()
    set(MYCROFT_LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "mycroft sys lib directory")
endif ()

# Set some local directories and files
set(MYCROFT_LOCAL_CONFIG   ${MYCROFT_LOCAL_BASE}/rc.py)     # config file
set(MYCROFT_LOCAL_PLUGINS  ${MYCROFT_LOCAL_BASE}/plugins)   # plugin directory
set(MYCROFT_LOCAL_SCRIPTS  ${MYCROFT_LOCAL_BASE}/scripts)   # simple scripts
set(MYCROFT_LOCAL_MODULES  ${MYCROFT_LOCAL_BASE}/modules)   # non-plugin python or c modules
set(MYCROFT_LOCAL_TEMPLATE ${MYCROFT_LOCAL_BASE}/templates) # template directory
set(MYCROFT_LOCAL_TYPES    ${MYCROFT_LOCAL_BASE}/types)     # template directory

# Set some local directories and files
set(MYCROFT_CONFIG   ${MYCROFT_DATA_PATH}/rc.py)     # config file
set(MYCROFT_PLUGINS  ${MYCROFT_DATA_PATH}/plugins)   # plugin directory
set(MYCROFT_SCRIPTS  ${MYCROFT_DATA_PATH}/scripts)   # simple scripts
set(MYCROFT_MODULES  ${MYCROFT_DATA_PATH}/modules)   # non-plugin python or c modules
set(MYCROFT_TEMPLATE ${MYCROFT_DATA_PATH}/templates) # template directory
set(MYCROFT_TYPES    ${MYCROFT_DATA_PATH}/types)     # template directory

# Set project directory locations
set(MYCROFT_CORE_DIR   ${CMAKE_SOURCE_DIR}/lib/core)
set(MYCROFT_CLIENT_DIR ${CMAKE_SOURCE_DIR}/src/client)
set(MYCROFT_TEST_DIR   ${CMAKE_SOURCE_DIR}/tests)
