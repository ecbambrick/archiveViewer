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
    sevenziparchiver.cpp \
    fileio.cpp \
    reversiblerand.cpp \
    imagelistfilter.cpp \
    imageview.cpp \
    image.cpp \
    playlist.cpp

HEADERS  += mainwindow.h \
    imagelist.h \
    directoryimagelist.h \
    archivedimagelist.h \
    sevenziparchiver.h \
    archiver.h \
    fileio.h \
    reversiblerand.h \
    imagelistfilter.h \
    imageview.h \
    image.h \
    playlist.h

RESOURCES += \
    resources.qrc
