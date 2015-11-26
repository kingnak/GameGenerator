#include "ggconditioneditorpane.h"
#include "ui_ggconditioneditorpane.h"
#include <view/gguicontroller.h>
#include <model/ggpage.h>

GGConditionEditorPane::GGConditionEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGConditionEditorPane),
    m_ctrl(NULL),
    m_page(NULL)
{
    ui->setupUi(this);
}

GGConditionEditorPane::~GGConditionEditorPane()
{
    delete ui;
}

void GGConditionEditorPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGConditionEditorPane::setPage(GGConditionPage *page)
{
    m_page = page;
    ui->wgtTrue->setConnection(page, GGConnectionSlot::TrueConnection);
    ui->wgtFalse->setConnection(page, GGConnectionSlot::FalseConnection);
}

void GGConditionEditorPane::connectPage()
{
    if (sender() == ui->wgtTrue) {
        m_ctrl->connectPageDirect(m_page, GGConnectionSlot::TrueConnection);
    } else if (sender() == ui->wgtFalse) {
        m_ctrl->connectPageDirect(m_page, GGConnectionSlot::FalseConnection);
    } else {
        Q_ASSERT(false);
    }
}
