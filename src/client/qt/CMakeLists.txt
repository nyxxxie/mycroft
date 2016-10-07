cmake_minimum_required(VERSION 2.8.11)

# Version
set(MYCROFT_CLIENT_QT_MAJOR_VERSION 0)
set(MYCROFT_CLIENT_QT_MAJOR_VERSION 1)

# Project name
project(${MYCROFT_CLIENT_QT_NAME} CXX)

# Source files
set(MYCROFT_CLIENT_QT_SRC
    main.cpp
    mainwindow.cpp
    maineditor.cpp
    hexeditor.cpp
    templateeditor.cpp
    templatemodel.cpp
)

# Save me from manually including the files ;)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# QT5 stuff
find_package(Qt5Widgets REQUIRED)
#qt5_wrap_ui(UI_HEADERS mainwindow.ui)
qt5_add_resources(QRCS resources.qrc)
set(CMAKE_AUTOMOC ON)

# Specify the exe to be built
add_executable(${MYCROFT_CLIENT_QT_NAME} ${MYCROFT_CLIENT_QT_SRC} ${UI_HEADERS} ${QRCS})
add_dependencies(${MYCROFT_CLIENT_QT_NAME} ${MYCROFT_CORE_NAME})

# Link libraries
target_link_libraries(${MYCROFT_CLIENT_QT_NAME} Qt5::Widgets ${MYCROFT_CORE_NAME})

# Enable some C++ features we need
target_compile_features(${MYCROFT_CLIENT_QT_NAME} PRIVATE cxx_generalized_initializers)
