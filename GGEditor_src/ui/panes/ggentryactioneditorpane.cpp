#include "ggentryactioneditorpane.h"
#include "ui_ggentryactioneditorpane.h"
#include <model/ggpage.h>
#include <view/gguicontroller.h>
#include <model/ggeditmodel.h>
#include <viewmodel/ggviewmodel.h>

GGEntryActionEditorPane::GGEntryActionEditorPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGEntryActionEditorPane),
    m_ctrl(NULL),
    m_page(NULL)
{
    ui->setupUi(this);

    connect(ui->wgtEntryAction, SIGNAL(actionUpdated(GGAction)), this, SLOT(updateEntryAction()));
}

GGEntryActionEditorPane::~GGEntryActionEditorPane()
{
    delete ui;
}

void GGEntryActionEditorPane::setController(GGUIController *ctrl)
{
    m_ctrl = ctrl;
}

void GGEntryActionEditorPane::setPage(GGEntryActionPage *page)
{
    m_page = page;
    if (page) {
        ui->wgtEntryAction->setVariables(m_ctrl->model()->editModel()->variableNames());
        ui->wgtEntryAction->setAction(page->entryAction());
    }
}

void GGEntryActionEditorPane::updateEntryAction()
{
    GGAction a = ui->wgtEntryAction->getAction();
    m_ctrl->changeEntryAction(m_page, a);
}
