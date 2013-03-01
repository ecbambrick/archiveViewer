#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T21:47:46
#
#-------------------------------------------------

QT       += core gui

TARGET = archiveViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    archiver.cpp \
    directoryimagelist.cpp \
    archivedimagelist.cpp

HEADERS  += mainwindow.h \
    archiver.h \
    imagelist.h \
    directoryimagelist.h \
    archivedimagelist.h

FORMS    += mainwindow.ui

INCLUDEPATH += \
    7z992

OTHER_FILES += \
    notes.txt

RESOURCES +=
