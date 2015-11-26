#include "ggactioneditorwidget.h"
#include "ui_ggactioneditorwidget.h"

GGActionEditorWidget::GGActionEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGActionEditorWidget)
{
    ui->setupUi(this);

    ui->cmbOp->clear();
    ui->cmbOp->addItem("", GGAction::None);
    ui->cmbOp->addItem("+=", GGAction::Add);
    ui->cmbOp->addItem("-=", GGAction::Sub);
    ui->cmbOp->addItem("*=", GGAction::Multiply);
    ui->cmbOp->addItem("/=", GGAction::Divide);
    ui->cmbOp->addItem("=", GGAction::Set);
    ui->cmbOp->addItem("X", GGAction::Unset);
}

GGActionEditorWidget::~GGActionEditorWidget()
{
    delete ui;
}

GGAction GGActionEditorWidget::getAction()
{
    return m_action;
}

void GGActionEditorWidget::setAction(GGAction action)
{
    m_action = action;
    ui->txtVar->setText(action.variableName());
    ui->txtVal->setText(action.value());
    int idx = ui->cmbOp->findData(action.type());
    if (idx < 0) idx = 0;
    ui->cmbOp->setCurrentIndex(idx);

    QString val;
    if (action.type() != GGAction::None) {
        if (action.type() == GGAction::Unset) {
            val = tr("Unset %1").arg(action.variableName());
        } else {
            val = QString("%1 %2 %3").arg(action.variableName(), ui->cmbOp->currentText(), action.value());
        }
    }
    ui->lblView->setText(val);
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
    updateAction();
    emit actionUpdated(m_action);
    setAction(m_action);
    ui->stackedWidget->setCurrentWidget(ui->pageView);
}

void GGActionEditorWidget::on_cmbOp_currentIndexChanged(int)
{
    int v = ui->cmbOp->currentData().toInt();
    switch (v) {
    case GGAction::None:
        ui->txtVar->setEnabled(false);
        // Fallthrough
    case GGAction::Unset:
        ui->txtVal->setEnabled(false);
        break;
    default:
        ui->txtVal->setEnabled(true);
        ui->txtVar->setEnabled(true);
    }
}

void GGActionEditorWidget::updateAction()
{
    m_action = GGAction(static_cast<GGAction::Type> (ui->cmbOp->currentData().toInt()), ui->txtVar->text(), ui->txtVal->text());
}
