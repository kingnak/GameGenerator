#include "ggactioneditorpane.h"
#include "ui_ggactioneditorpane.h"
#include <model/ggpage.h>
#include <model/gglink.h>
#include <view/gguicontroller.h>

GGActionEditorPane::GGActionEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGActionEditorPane),
    m_ctrl(NULL),
    m_page(NULL)
{
    ui->setupUi(this);

    connect(ui->wgtLink, SIGNAL(updateCaption(GGPage*,GGConnectionSlot,QString)), this, SLOT(updateLink()));
    connect(ui->wgtLink, SIGNAL(updateAction(GGPage*,GGConnectionSlot,GGAction)), this, SLOT(updateLink()));
    connect(ui->wgtLink, SIGNAL(activated(GGPage*,GGConnectionSlot)), this, SLOT(connectAction()));
}

GGActionEditorPane::~GGActionEditorPane()
{
    delete ui;
}

void GGActionEditorPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGActionEditorPane::setPage(GGActionPage *page)
{
    m_page = page;
    if (page) {
        ui->wgtLink->setConnection(m_page, GGConnectionSlot::ActionConnection);
    }
}

void GGActionEditorPane::updateLink()
{
    GGLink l;
    l.setName(ui->wgtLink->getCaption());
    l.setAction(ui->wgtLink->getAction());
    m_ctrl->changeLink(m_page, GGConnectionSlot::ActionConnection, l);
}

void GGActionEditorPane::connectAction()
{
    m_ctrl->connectPageDirect(m_page, GGConnectionSlot::ActionConnection);
}
