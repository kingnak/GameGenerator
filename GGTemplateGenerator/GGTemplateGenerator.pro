#-------------------------------------------------
#
# Project created by QtCreator 2016-05-26T14:15:12
#
#-------------------------------------------------

QT       += core gui

TARGET = GGTemplateGenerator
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../GGEditor_src/generator

SOURCES += ggtemplategeneratorplugin.cpp

HEADERS += ggtemplategeneratorplugin.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}
