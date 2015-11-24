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
    command/ggeditcommandfactory.cpp \
    command/ggabstractcommand.cpp \
    model/ggconnectionslot.cpp \
    command/ggmodelpagecommands.cpp \
    command/ggmodelconnectioncommands.cpp \
    command/ggcommandstack.cpp \
    command/ggcommandgroup.cpp \
    viewmodel/ggviewmodel.cpp \
    viewmodel/ggviewpage.cpp \
    viewmodel/ggviewconnection.cpp \
    viewcommand/ggviewcommands.cpp \
    viewcommand/ggviewcommandfactory.cpp \
    view/ggeditorscene.cpp \
    view/ggpageitem.cpp \
    view/ggselectionitem.cpp \
    view/gguicontroller.cpp \
    view/ggconnectionitem.cpp

HEADERS += ggmainwindow.h \
    model/ggeditmodel.h \
    command/ggeditcommandfactory.h \
    command/ggabstractcommand.h \
    command/ggabstractmodelcommand.h \
    model/ggconnectionslot.h \
    command/ggmodelpagecommands.h \
    command/ggmodelconnectioncommands.h \
    command/ggcommandstack.h \
    command/ggcommandgroup.h \
    command/ggabstractmodelcommandfactory.h \
    viewmodel/ggviewmodel.h \
    viewmodel/ggviewpage.h \
    viewmodel/ggviewconnection.h \
    viewcommand/ggviewcommands.h \
    viewcommand/ggviewcommandfactory.h \
    view/ggeditorscene.h \
    view/ggpageitem.h \
    view/ggselectionitem.h \
    view/gguicontroller.h \
    view/ggconnectionitem.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gg_core/release/ -lgg_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gg_core/debug/ -lgg_core
else:unix: LIBS += -L$$OUT_PWD/../gg_core/ -lgg_core

INCLUDEPATH += $$PWD/../gg_core
DEPENDPATH += $$PWD/../gg_core

include(../defaults.pri)
