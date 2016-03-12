#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T12:40:35
#
#-------------------------------------------------

QT       += widgets

TARGET = GGEditor_src
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
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
    view/ggconnectionitem.cpp \
    ggmainwindow.cpp \
    view/ggeditorview.cpp \
    ui/components/ggconnectioneditorwidget.cpp \
    ui/dialogs/ggchoseconnectionslotdlg.cpp \
    ui/ggpageeditpanel.cpp \
    ui/panes/ggconditioneditorpane.cpp \
    ui/panes/ggcontenteditorpane.cpp \
    ui/dialogs/ggeditcontentelementdialog.cpp \
    ui/components/ggactioneditorwidget.cpp \
    ui/panes/ggactioneditorpane.cpp \
    ui/panes/ggdecisioneditorpane.cpp \
    ui/components/ggeditcontentelementwidget.cpp \
    ui/panes/ggmappingeditorpane.cpp \
    ui/components/ggconnectionlisteditorwidget.cpp \
    ui/dialogs/ggeditcontentmappingdialog.cpp \
    view/ggmappingscene.cpp \
    view/ggmappinguicontroller.cpp \
    ui/basic/ggmouselabel.cpp

HEADERS += \
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
    view/ggconnectionitem.h \
    ggmainwindow.h \
    view/ggeditorview.h \
    ui/components/ggconnectioneditorwidget.h \
    ui/dialogs/ggchoseconnectionslotdlg.h \
    ui/ggpageeditpanel.h \
	ui/panes/ggconditioneditorpane.h \
    ui/panes/ggcontenteditorpane.h \
    ui/dialogs/ggeditcontentelementdialog.h \
    ui/components/ggactioneditorwidget.h \
    ui/panes/ggactioneditorpane.h \
    ui/panes/ggdecisioneditorpane.h \
    ui/components/ggeditcontentelementwidget.h \
    ui/panes/ggmappingeditorpane.h \
    ui/components/ggconnectionlisteditorwidget.h \
    view/ggresizableitem.h \
    ui/dialogs/ggeditcontentmappingdialog.h \
    view/ggmappingscene.h \
    view/ggmappinguicontroller.h \
    ui/basic/ggmouselabel.h
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

FORMS += \
    ggmainwindow.ui \
    ui/components/ggconnectioneditorwidget.ui \
    ui/dialogs/ggchoseconnectionslotdlg.ui \
    ui/ggpageeditpanel.ui \
	ui/panes/ggconditioneditorpane.ui \
    ui/panes/ggcontenteditorpane.ui \
    ui/dialogs/ggeditcontentelementdialog.ui \
    ui/components/ggactioneditorwidget.ui \
    ui/panes/ggactioneditorpane.ui \
    ui/panes/ggdecisioneditorpane.ui \
    ui/components/ggeditcontentelementwidget.ui \
    ui/panes/ggmappingeditorpane.ui \
    ui/components/ggconnectionlisteditorwidget.ui \
    ui/dialogs/ggeditcontentmappingdialog.ui

RESOURCES += \
    ui/resources.qrc
