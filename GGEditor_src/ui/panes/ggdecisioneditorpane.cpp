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
    ui->wgtDecisions->setLayout(new QVBoxLayout);
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

    cleanDecisions();
    if (!page) return;
    for (int i = 0; i < page->getDecisionLinks().size(); ++i) {
        GGConnectionEditorWidget *w = new GGConnectionEditorWidget;
        w->setConnection(page, GGConnectionSlot(GGConnectionSlot::DecisionConnection, i));
        w->setDeletable(true);
        connect(w, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SLOT(deleteLink()));
        connect(w, SIGNAL(activated(GGPage*,GGConnectionSlot)), this, SLOT(connectLink()));
        ui->wgtDecisions->layout()->addWidget(w);
    }
    ui->wgtDecisions->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void GGDecisionEditorPane::deleteLink()
{
    GGConnectionSlot s = static_cast<GGConnectionEditorWidget*> (sender())->slot();
    m_ctrl->removeDecisionLink(m_page, s.index());
}

void GGDecisionEditorPane::connectLink()
{
    GGConnectionSlot s = static_cast<GGConnectionEditorWidget*> (sender())->slot();
    m_ctrl->connectPageDirect(m_page, s);
}

void GGDecisionEditorPane::on_btnAdd_clicked()
{
    m_ctrl->addDecisionLink(m_page);
}

void GGDecisionEditorPane::cleanDecisions()
{
    while (ui->wgtDecisions->layout()->count()) {
        delete ui->wgtDecisions->layout()->takeAt(0);
    }
    QList<GGConnectionEditorWidget*> l = ui->wgtDecisions->findChildren<GGConnectionEditorWidget*>();
    qDeleteAll(l);
}
