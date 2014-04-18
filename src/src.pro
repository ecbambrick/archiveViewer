#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T21:47:46
#
#-------------------------------------------------

QT += core gui widgets

TEMPLATE = app
TARGET = archiveViewer
RC_FILE = app.rc

QMAKE_CXXFLAGS += -std=c++0x
CONFIG += c++11

SOURCES	+= \
    main.cpp \
    mainwindow.cpp \
    directoryimagelist.cpp \
    archivedimagelist.cpp \
    sevenziparchiver.cpp \
    fileio.cpp \
    reversiblerand.cpp \
    imagelistfilter.cpp \
    imageview.cpp \
    image.cpp \
    filter.cpp

HEADERS += \
    mainwindow.h \
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
    imagesource.h \
    filter.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    default.qss

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../quazip/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../quazip/debug/ -lquazip
else:unix: LIBS += -L$$OUT_PWD/../quazip/ -lquazip

INCLUDEPATH += $$PWD/../quazip
DEPENDPATH += $$PWD/../quazip
