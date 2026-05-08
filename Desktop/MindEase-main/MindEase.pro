QT       += core gui widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -Wno-implicit-function-declaration

TARGET = MindEase
TEMPLATE = app

INCLUDEPATH += \
    . \
    app \
    core \
    screens \
    models \
    storage

SOURCES += \
    app/main.cpp \
    app/mainwindow.cpp \
    core/lucideicons.cpp \
    core/screen.cpp \
    screens/home.cpp \
    screens/recommendations.cpp \
    screens/toolkit.cpp \
    screens/journal.cpp \
    models/journalentry.cpp \
    storage/journalstorage.cpp

HEADERS += \
    app/mainwindow.h \
    core/lucideicons.h \
    core/screen.h \
    screens/home.h \
    screens/recommendations.h \
    screens/toolkit.h \
    screens/journal.h \
    models/journalentry.h \
    storage/journalstorage.h

RESOURCES += \
    resources/resources.qrc
