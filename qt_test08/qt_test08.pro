#-------------------------------------------------
#
# Project created by QtCreator 2014-05-25T18:03:55
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = qt_test08
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/5.3/msvc2013_64/lib/ -lQt5Sql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/5.3/msvc2013_64/lib/ -lQt5Sqld
else:unix: LIBS += -L$$PWD/../../../../Qt/5.3/msvc2013_64/lib/ -lQt5Sql

INCLUDEPATH += $$PWD/../../../../Qt/5.3/msvc2013_64/lib
DEPENDPATH += $$PWD/../../../../Qt/5.3/msvc2013_64/lib


INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

