#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T10:45:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mycroft
TEMPLATE = app


SOURCES += main.cpp\
        mycroft.cpp \
    templateeditor.cpp \
    templatemodel.cpp \
    hexeditor.cpp \
    mainhexeditor.cpp

HEADERS  += mycroft.h \
    templateeditor.h \
    templatemodel.h \
    hexeditor.h \
    mainhexeditor.h

FORMS    += mycroft.ui \
    mainhexeditor.ui

DISTFILES += \
    CMakeLists.txt
