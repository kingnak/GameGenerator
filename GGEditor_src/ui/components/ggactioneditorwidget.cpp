#include "ggactioneditorwidget.h"
#include "ui_ggactioneditorwidget.h"

GGActionEditorWidget::GGActionEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGActionEditorWidget)
{
    ui->setupUi(this);

    ui->cmbOp->clear();
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::None), GGAction::None);
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::Add), GGAction::Add);
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::Sub), GGAction::Sub);
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::Multiply), GGAction::Multiply);
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::Divide), GGAction::Divide);
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::Set), GGAction::Set);
    ui->cmbOp->addItem(GGAction::getActionStringForType(GGAction::Unset), GGAction::Unset);
}

GGActionEditorWidget::~GGActionEditorWidget()
{
    delete ui;
}

GGAction GGActionEditorWidget::getAction()
{
    return m_action;
}

void GGActionEditorWidget::setVariables(QStringList vars)
{
    qSort(vars);
    ui->cmbVar->clear();
    ui->cmbVar->addItem("");
    ui->cmbVar->addItems(vars);
}

void GGActionEditorWidget::setAction(const GGAction &action)
{
    m_action = action;
    ui->cmbVar->setEditText(action.variableName());
    ui->txtVal->setText(action.value());
    int idx = ui->cmbOp->findData(action.type());
    if (idx < 0) idx = 0;
    ui->cmbOp->setCurrentIndex(idx);
    ui->lblView->setText(m_action.toString());
}

void GGActionEditorWidget::setEditable(bool editable)
{
    ui->btnEdit->setVisible(editable);
    on_btnAbort_clicked();
}

void GGActionEditorWidget::on_btnEdit_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageEdit);
}

void GGActionEditorWidget::on_btnAbort_clicked()
{
    setAction(m_action);
    ui->stackedWidget->setCurrentWidget(ui->pageView);
}

void GGActionEditorWidget::on_btnOk_clicked()
{
    GGAction a = buildAction();
    setAction(a);
    ui->stackedWidget->setCurrentWidget(ui->pageView);
    emit actionUpdated(a);
}

void GGActionEditorWidget::on_cmbOp_currentIndexChanged(int)
{
    int v = ui->cmbOp->currentData().toInt();
    switch (v) {
    case GGAction::None:
        ui->cmbVar->setEnabled(false);
        // Fallthrough
    case GGAction::Unset:
        ui->txtVal->setEnabled(false);
        break;
    default:
        ui->txtVal->setEnabled(true);
        ui->cmbVar->setEnabled(true);
    }
}

GGAction GGActionEditorWidget::buildAction()
{
    return GGAction(static_cast<GGAction::Type> (ui->cmbOp->currentData().toInt()), ui->cmbVar->currentText(), ui->txtVal->text());
}
