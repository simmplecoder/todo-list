#-------------------------------------------------
#
# Project created by QtCreator 2017-01-14T22:57:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = todolist
TEMPLATE = app
CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    addnewdialog.cpp \
    placeholder.cpp

HEADERS  += mainwindow.h \
    addnewdialog.h \
    placeholder.h

FORMS    += mainwindow.ui \
    addnewdialog.ui
