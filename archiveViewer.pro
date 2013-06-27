#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T21:47:46
#
#-------------------------------------------------

QT       += core gui

TARGET = archiveViewer
TEMPLATE = app
RC_FILE = app.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    directoryimagelist.cpp \
    archivedimagelist.cpp \
    sevenziparchiver.cpp

HEADERS  += mainwindow.h \
    imagelist.h \
    directoryimagelist.h \
    archivedimagelist.h \
    sevenziparchiver.h \
    archiver.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
