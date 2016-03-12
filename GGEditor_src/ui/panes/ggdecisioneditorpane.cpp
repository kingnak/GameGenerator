#include "ggdecisioneditorpane.h"
#include "ui_ggdecisioneditorpane.h"
#include <model/ggpage.h>
#include <view/gguicontroller.h>
#include "../components/ggconnectioneditorwidget.h"

GGDecisionEditorPane::GGDecisionEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGDecisionEditorPane)
{
    ui->setupUi(this);
    connect(ui->wgtDecisions, SIGNAL(connectConnection(GGPage*,GGConnectionSlot)), this, SLOT(connectLink(GGPage*,GGConnectionSlot)));
    connect(ui->wgtDecisions, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SLOT(deleteLink(GGPage*,GGConnectionSlot)));
    connect(ui->wgtDecisions, SIGNAL(updateLinkCaption(GGPage*,GGConnectionSlot,QString)), this, SLOT(updateLinkCaption(GGPage*,GGConnectionSlot,QString)));
    connect(ui->wgtDecisions, SIGNAL(updateLinkAction(GGPage*,GGConnectionSlot,GGAction)), this, SLOT(updateLinkAction(GGPage*,GGConnectionSlot,GGAction)));
}

GGDecisionEditorPane::~GGDecisionEditorPane()
{
    delete ui;
}

void GGDecisionEditorPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGDecisionEditorPane::setPage(GGDecisionPage *page)
{
    m_page = page;
    QList<GGConnectionSlot> slts = GGConnectionSlot::enumerateConnections(page, GGConnectionSlot::DecisionConnection);
    ui->wgtDecisions->setConnections(m_page, slts);
}

void GGDecisionEditorPane::updateLinkCaption(GGPage *, const GGConnectionSlot &slt, const QString &cap)
{
    GGLink l = m_page->getDecisionLinks()[slt.index()];
    l.setName(cap);
    m_ctrl->changeLink(m_page, slt, l);
}

void GGDecisionEditorPane::updateLinkAction(GGPage *, const GGConnectionSlot &slt, const GGAction &act)
{
    GGLink l = m_page->getDecisionLinks()[slt.index()];
    l.setAction(act);
    m_ctrl->changeLink(m_page, slt, l);
}

void GGDecisionEditorPane::deleteLink(GGPage *, const GGConnectionSlot &slt)
{
    m_ctrl->removeDecisionLink(m_page, slt.index());
}

void GGDecisionEditorPane::connectLink(GGPage *, const GGConnectionSlot &slt)
{
    m_ctrl->connectPageDirect(m_page, slt);
}

void GGDecisionEditorPane::on_btnAdd_clicked()
{
    m_ctrl->addDecisionLink(m_page);
}
