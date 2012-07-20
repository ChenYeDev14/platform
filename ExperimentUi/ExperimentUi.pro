#-------------------------------------------------
#
# Project created by QtCreator 2012-07-01T20:39:27
#
#-------------------------------------------------

QT       += core gui
QT       += core network
QT       += thread

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExperimentUi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../NetBattle.cpp \
    ../NormalBattle.cpp \
    ../replayfile.cpp

HEADERS  += mainwindow.h \
    ../NetBattle.h \
    ../NormalBattle.h \
    ../NetBattle.h \
    ../NormalBattle.h \
    ../Status.h \
    ../Logic.h \
    ../Basic.h \
    ../replayfile.h

FORMS    += mainwindow.ui
