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
    filter.cpp \
    imageinfo.cpp \
    imagesource.cpp \
    imageview.cpp \
    localimagesource.cpp \
    main.cpp \
    mainwindow.cpp \
    quazipimagesource.cpp \
    utility.cpp \
    playlist.cpp

HEADERS += \
    filter.h \
    imageinfo.h \
    imagesource.h \
    imageview.h \
    localimagesource.h \
    mainwindow.h \
    quazipimagesource.h \
    utility.h \
    playlist.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    default.qss

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../quazip/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../quazip/debug/ -lquazip
else:unix: LIBS += -L$$OUT_PWD/../quazip/ -lquazip

INCLUDEPATH += $$PWD/../quazip
DEPENDPATH += $$PWD/../quazip
