#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T12:35:44
#
#-------------------------------------------------

#QT       -= gui

TARGET = gg_core
TEMPLATE = lib

DEFINES += GG_CORE_LIBRARY

SOURCES += \
    model/ggabstractmodel.cpp \
    model/ggconnection.cpp \
    model/ggpage.cpp \
    model/ggruntimemodel.cpp \
    model/ggsimplefactory.cpp \
    model/ggcontentelement.cpp \
    model/gglink.cpp \
    model/ggmappedlink.cpp \
    model/ggvariable.cpp \
    model/ggaction.cpp \
    model/ggcondition.cpp

HEADERS +=\
        gg_core_global.h \
    model/ggabstractmodel.h \
    model/ggconnection.h \
    model/ggpage.h \
    model/ggruntimemodel.h \
    gg_definitions.h \
    model/ggabstractfactory.h \
    model/ggsimplefactory.h \
    model/ggcontentelement.h \
    model/gglink.h \
    model/ggmappedlink.h \
    model/ggvariable.h \
    model/ggaction.h \
    model/ggcondition.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(../defaults.pri)

