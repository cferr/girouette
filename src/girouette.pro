# -------------------------------------------------
# Project created by QtCreator 2011-03-12T20:12:13
# -------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core
QT += xml
TARGET = girouette
TEMPLATE = app
SOURCES += main.cpp \
    signwindow.cpp \
    fonteditor.cpp \
    clicklabel.cpp \
    fontfile.cpp \
    fontmgmt.cpp \
    displaysettings.cpp \
    settingsfile.cpp \
    generalsettings.cpp \
    common.cpp \
    girfile.cpp \
    customsettings.cpp
HEADERS += signwindow.h \
    fonteditor.h \
    clicklabel.h \
    fontfile.h \
    fontmgmt.h \
    displaysettings.h \
    settingsfile.h \
    generalsettings.h \
    common.h \
    girfile.h \
    customsettings.h
FORMS += signwindow.ui \
    fonteditor.ui \
    fontmgmt.ui \
    displaysettings.ui \
    generalsettings.ui
RESOURCES += elements.qrc
TRANSLATIONS = girouette_fr.ts
