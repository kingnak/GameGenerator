#-------------------------------------------------
#
# Project created by QtCreator 2016-05-26T14:15:12
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = GGTemplateGenerator
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../GGEditor_src/generator

SOURCES += ggtemplategeneratorplugin.cpp \
    settingsui.cpp

HEADERS += ggtemplategeneratorplugin.h \
    settingsui.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

FORMS += \
    settingsui.ui

RESOURCES += \
    templategenerator.qrc
