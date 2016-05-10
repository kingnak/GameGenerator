#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T12:42:32
#
#-------------------------------------------------

QT       += testlib

#QT       -= gui

TARGET = tst_ggeditor_testtest
CONFIG   += console
CONFIG	 += qttest
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    tst_ggeditor_basicmodeltest.cpp \
    testmain.cpp \
    tst_ggeditor_basiccommandtest.cpp \
    tst_ggeditor_basicobjecttest.cpp \
    tst_ggeditor_connectionslottest.cpp \
    tst_ggeditor_modelcommandtest.cpp \
    tst_ggeditor_viewcommandtest.cpp \
    testsignalchecker.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GGEditor_src/release/ -lGGEditor_src
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GGEditor_src/debug/ -lGGEditor_src
else:unix: LIBS += -L$$OUT_PWD/../GGEditor_src/ -lGGEditor_src

INCLUDEPATH += $$PWD/../GGEditor_src
DEPENDPATH += $$PWD/../GGEditor_src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GGEditor_src/release/libGGEditor_src.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GGEditor_src/debug/libGGEditor_src.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GGEditor_src/release/GGEditor_src.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GGEditor_src/debug/GGEditor_src.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../GGEditor_src/libGGEditor_src.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gg_core/release/ -lgg_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../gg_core/debug/ -lgg_core
else:unix: LIBS += -L$$OUT_PWD/../gg_core/ -lgg_core

include(../defaults.pri)

INCLUDEPATH += $$PWD/../gg_core
DEPENDPATH += $$PWD/../gg_core

HEADERS += \
    tst_ggeditor_basicmodeltest.h \
    tst_ggeditor_basiccommandtest.h \
    tst_ggeditor_basicobjecttest.h \
    tst_ggeditor_connectionslottest.h \
    tst_ggeditor_modelcommandtest.h \
    tst_ggeditor_viewcommandtest.h \
    testsignalchecker.h
