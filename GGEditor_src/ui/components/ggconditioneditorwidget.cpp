#include "ggconditioneditorwidget.h"
#include "ui_ggconditioneditorwidget.h"
#include <model/ggcondition.h>
#include <ui/basic/ggstringlistvalidator.h>

GGConditionEditorWidget::GGConditionEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGConditionEditorWidget)
{
    ui->setupUi(this);

    ui->cmbOp->clear();
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::None), GGCondition::None);
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::Equals), GGCondition::Equals);
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::NotEquals), GGCondition::NotEquals);
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::LessThen), GGCondition::LessThen);
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::LessEqual), GGCondition::LessEqual);
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::GreaterThen), GGCondition::GreaterThen);
    ui->cmbOp->addItem(GGCondition::getConditionStringForType(GGCondition::GreaterEquals), GGCondition::GreaterEquals);

    m_varValidator = new GGStringListValidator(this);
    ui->cmbVar->setValidator(m_varValidator);
}

GGConditionEditorWidget::~GGConditionEditorWidget()
{
    delete ui;
}

void GGConditionEditorWidget::setVariables(QStringList vars)
{
    if (m_varCache != vars) {
        m_varCache = vars;
        qSort(vars);
        m_varValidator->setStrings(vars);
        ui->cmbVar->clear();
        ui->cmbVar->addItem("");
        ui->cmbVar->addItems(vars);
    } else {
        ui->cmbVar->setCurrentIndex(0);
    }
}

void GGConditionEditorWidget::setCondition(const GGCondition &cond)
{
    m_cond = cond;
    ui->stackedWidget->setCurrentWidget(ui->pageDisplay);
    ui->cmbVar->setEditText(cond.variableName());
    ui->txtValue->setText(cond.value());
    int idx = ui->cmbOp->findData(cond.type());
    if (idx < 0) idx = 0;
    ui->cmbOp->setCurrentIndex(idx);
    ui->lblDisplay->setText(cond.toString());
}

void GGConditionEditorWidget::setEditable(bool editable)
{
    ui->btnEdit->setVisible(editable);
    on_btnCancel_clicked();
}

void GGConditionEditorWidget::on_btnEdit_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageEdit);
}

void GGConditionEditorWidget::on_btnCancel_clicked()
{
    setCondition(m_cond);
    ui->stackedWidget->setCurrentWidget(ui->pageDisplay);
}

void GGConditionEditorWidget::on_btnOk_clicked()
{
    GGCondition c = buildCondition();
    setCondition(c);
    ui->stackedWidget->setCurrentWidget(ui->pageDisplay);
    emit conditionUpdated(c);
}

void GGConditionEditorWidget::on_cmbOp_currentIndexChanged(int idx)
{
    bool disable = (ui->cmbOp->itemData(idx).toInt() == GGCondition::None);
    ui->txtValue->setDisabled(disable);
    ui->cmbVar->setDisabled(disable);
}

GGCondition GGConditionEditorWidget::buildCondition()
{
    GGCondition c(static_cast<GGCondition::Type> (ui->cmbOp->currentData().toInt()), ui->cmbVar->currentText(), ui->txtValue->text());
    return c;
}
