#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T12:40:35
#
#-------------------------------------------------

QT       += widgets

TARGET = GGEditor_src
TEMPLATE = lib
CONFIG += staticlib

SOURCES += ggmainwindow.cpp \
    model/ggeditmodel.cpp \
    command/ggmodelcommands.cpp \
    command/ggeditcommandfactory.cpp \
    command/ggabstractcommand.cpp \
    model/ggconnectionslot.cpp

HEADERS += ggmainwindow.h \
    model/ggeditmodel.h \
    command/ggabstractcommandfactory.h \
    command/ggmodelcommands.h \
    command/ggeditcommandfactory.h \
    command/ggabstractcommand.h \
    command/ggabstractmodelcommand.h \
    model/ggconnectionslot.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gg_core/release/ -lgg_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gg_core/debug/ -lgg_core
else:unix: LIBS += -L$$OUT_PWD/../gg_core/ -lgg_core

INCLUDEPATH += $$PWD/../gg_core
DEPENDPATH += $$PWD/../gg_core
