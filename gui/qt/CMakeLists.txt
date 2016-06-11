cmake_minimum_required(VERSION 2.8.11)

# Project name
set(PROJECT_NAME mycroft-qt)
project(${PROJECT_NAME})

# Source files
set(SRC
    main.cpp
    mainwindow.cpp
    maineditor.cpp
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
add_executable(${PROJECT_NAME} ${SRC} ${UI_HEADERS} ${QRCS})
target_link_libraries(${PROJECT_NAME}  Qt5::Widgets)
