# FileMonitorApp.pro: Qt project file to configure the build process
# Run using: qmake FileMonitorApp.pro && make
# Executed by qmake to generate Makefile for building the application

QT += core gui widgets statemachine


CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    FileCreator.cpp \
    FileDeleter.cpp \
    FolderMonitor.cpp

HEADERS += \
    mainwindow.h \
    FileCreator.h \
    FileDeleter.h \
    FolderMonitor.h

TARGET = FileMonitorApp
