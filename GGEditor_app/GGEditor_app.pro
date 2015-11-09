#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T12:39:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GGEditor_app
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

FORMS    +=

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

include(../defaults.pri)
