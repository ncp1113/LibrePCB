#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T12:53:17
#
#-------------------------------------------------

TEMPLATE = lib
TARGET = librepcblibraryeditor

# Set the path for the generated binary
GENERATED_DIR = ../../../generated

# Use common project definitions
include(../../../common.pri)

QT += core widgets xml sql printsupport

CONFIG += staticlib

INCLUDEPATH += \
    ../../

SOURCES += \
    libraryeditor.cpp \
    common/editorwidgetbase.cpp

HEADERS += \
    libraryeditor.h \
    common/editorwidgetbase.h

FORMS += \
    libraryeditor.ui

