cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CLIENT_QT_MAJOR_VERSION 0)
set(MYCROFT_CLIENT_QT_MAJOR_VERSION 1)

# Project name
project(${MYCROFT_CLIENT_QT_NAME} CXX)

# Source files
set(MYCROFT_CLIENT_QT_SRC
    main.cpp
    mycroft.cpp
    mainhexeditor.cpp
    hexeditor.cpp
    templateeditor.cpp
    projectview.cpp
)

# UI files
set(MYCROFT_CLIENT_QT_UI
    mycroft.ui
    mainhexeditor.ui
)

# Save me from manually including the files ;)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

# QT5 stuff
find_package(Qt5Widgets REQUIRED)
qt5_wrap_ui(UI_HEADERS ${MYCROFT_CLIENT_QT_UI})
qt5_add_resources(QRCS resources.qrc)
set(CMAKE_AUTOMOC ON)

# Specify the exe to be built
add_executable(${MYCROFT_CLIENT_QT_NAME} ${MYCROFT_CLIENT_QT_SRC} ${UI_HEADERS} ${QRCS})
add_dependencies(${MYCROFT_CLIENT_QT_NAME} ${MYCROFT_CORE_NAME})

# Link libraries
target_link_libraries(${MYCROFT_CLIENT_QT_NAME} Qt5::Widgets ${MYCROFT_CORE_NAME})

# Enable some C++ features we need
target_compile_features(${MYCROFT_CLIENT_QT_NAME} PRIVATE cxx_generalized_initializers)

# Set up install location
install(TARGETS ${MYCROFT_CLIENT_QT_NAME} RUNTIME DESTINATION bin)
