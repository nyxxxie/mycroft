cmake_minimum_required(VERSION 2.8.11)

#add_subdirectory(largefile)

# Create a variable that tells us where our resources are
set(TEST_FILE_SRC_RES ${CMAKE_CURRENT_SOURCE_DIR}/res)
set(TEST_FILE_BIN_RES ${CMAKE_CURRENT_BINARY_DIR}/res)

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

create_test(file SRC
    main.cpp
    file.cpp
    LIBS ${MYCROFT_CORE_NAME} ${OPENSSL_LIBRARIES}
    COPYFILES
)
