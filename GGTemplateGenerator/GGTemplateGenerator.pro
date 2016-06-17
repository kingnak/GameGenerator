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
    settingsui.cpp \
    ggtemplategenerator.cpp

HEADERS += ggtemplategeneratorplugin.h \
    settingsui.h \
    ggtemplategenerator.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

FORMS += \
    settingsui.ui

RESOURCES += \
    templategenerator.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gg_core/release/ -lgg_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gg_core/debug/ -lgg_core
else:unix: LIBS += -L$$OUT_PWD/../gg_core/ -lgg_core

INCLUDEPATH += $$PWD/../gg_core
DEPENDPATH += $$PWD/../gg_core
