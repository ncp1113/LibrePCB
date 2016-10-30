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
    common/categorychooserdialog.cpp \
    common/categorylisteditorwidget.cpp \
    common/categorytreelabeltextbuilder.cpp \
    common/editorwidgetbase.cpp \
    cmpcat/componentcategoryeditorwidget.cpp \
    pkgcat/packagecategoryeditorwidget.cpp \
    sym/symboleditorwidget.cpp

HEADERS += \
    libraryeditor.h \
    common/categorychooserdialog.h \
    common/categorylisteditorwidget.h \
    common/categorytreelabeltextbuilder.h \
    common/editorwidgetbase.h \
    cmpcat/componentcategoryeditorwidget.h \
    pkgcat/packagecategoryeditorwidget.h \
    sym/symboleditorwidget.h

FORMS += \
    libraryeditor.ui \
    common/categorychooserdialog.ui \
    common/categorylisteditorwidget.ui \
    cmpcat/componentcategoryeditorwidget.ui \
    pkgcat/packagecategoryeditorwidget.ui \
    sym/symboleditorwidget.ui

