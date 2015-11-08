# include user-defined things in every qmake project
exists(custom.pri):include(custom.pri)

# set prefix for "make install"
isEmpty(PREFIX):PREFIX = /usr/local

# set destination path for generated files
isEmpty(GENERATED_DIR):GENERATED_DIR = generated

# set destination path for binaries and libraries
macx:DESTDIR = $${GENERATED_DIR}/mac
unix:!macx:DESTDIR = $${GENERATED_DIR}/unix
win32:DESTDIR = $${GENERATED_DIR}/windows

# use separate folders for different types of files
OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui

#is qt version sufficient
lessThan(QT_MAJOR_VERSION, 5) {
    error("Qt version $$[QT_VERSION] is too old, should be version 5.2 or newer!")
} else {
    lessThan(QT_MINOR_VERSION, 2) {
        error("Qt version $$[QT_VERSION] is too old, should be version 5.2 or newer!")
    }
}

# c++11 is obligatory!
CONFIG += c++11

# enable compiler warnings
CONFIG += warn_on
QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -Weffc++ -pedantic
QMAKE_CXXFLAGS_RELEASE += -Werror

# mark Qt include paths as system paths to suppress compiler warnings caused by Qt's header files
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtOpenGL
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWebKitWidgets
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtXml
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtPrintSupport
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtSql

# mark the UI output directory as system path to suppress compiler warnings caused by UI header files
QMAKE_CXXFLAGS += -isystem $${UI_HEADERS_DIR}

# Define the application version
DEFINES += APP_VERSION_MAJOR=0
DEFINES += APP_VERSION_MINOR=1
DEFINES += APP_VERSION_PATCH=0
