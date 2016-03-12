#include "ggconnectioneditorwidget.h"
#include "ui_ggconnectioneditorwidget.h"
#include <model/ggabstractmodel.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGConnectionEditorWidget::GGConnectionEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGConnectionEditorWidget),
    m_page(NULL),
    m_slot(GGConnectionSlot::NoConnection),
    m_fields(AllFields),
    m_actions(DefaultActions)/*,
    m_editable(true)*/
{
    ui->setupUi(this);
//    ui->btnDelete->setVisible(false);
    connect(ui->wgtAction, SIGNAL(actionUpdated(GGAction)), this, SLOT(actionUpdated()));
    connect(ui->lblNumber, SIGNAL(hoverEnter()), this, SLOT(numberHoverEnter()));
    connect(ui->lblNumber, SIGNAL(hoverLeave()), this, SLOT(numberHoverLeave()));

    m_numberBaseStyle = ui->lblNumber->styleSheet();
    m_numberHoverStyle = m_numberBaseStyle + "\ncolor: rgb(255, 255, 255);";
}

GGConnectionEditorWidget::~GGConnectionEditorWidget()
{
    delete ui;
}

void GGConnectionEditorWidget::setFields(int f)
{
    m_fields = static_cast<Fields>(f);
    setConnection(m_page, m_slot);
}

void GGConnectionEditorWidget::setActions(int a)
{
    m_actions = static_cast<ConnectionActions>(a);
    if (m_actions.testFlag(Connect) && m_actions.testFlag(Select)) {
        m_actions = m_actions & (~Select);
    }
    setConnection(m_page, m_slot);
}

GGConnectionSlot GGConnectionEditorWidget::slot()
{
    return m_slot;
}

GGPage *GGConnectionEditorWidget::page()
{
    return m_page;
}

GGAction GGConnectionEditorWidget::getAction()
{
    return ui->wgtAction->getAction();
}

QString GGConnectionEditorWidget::getCaption()
{
    return ui->txtCaption->text();
}

void GGConnectionEditorWidget::setConnection(GGPage *page, GGConnectionSlot slot)
{
    m_page = page;
    m_slot = slot;
    ui->txtCaption->setEnabled(false);
    ui->txtCaption->setText("");
    ui->lblConnectedPage->setText("");

    ui->lblType->setVisible(m_fields.testFlag(Type));
    ui->lblNumber->setVisible(false);
    ui->wgtAction->setVisible(false);
    ui->txtCaption->setVisible(false);
    ui->btnActivate->setChecked(false);
    ui->btnActivate->setCheckable(m_actions.testFlag(Select));
    ui->btnActivate->setEnabled(m_actions.testFlag(Select) || m_actions.testFlag(Connect));

    if (slot.isLink()) {
        if (m_fields.testFlag(Action)) ui->wgtAction->setVisible(true);
        if (m_fields.testFlag(Caption)) ui->txtCaption->setVisible(true);
    }

    ui->txtCaption->setEnabled(m_actions.testFlag(Edit));
    ui->wgtAction->setEditable(m_actions.testFlag(Edit));
    ui->btnDelete->setEnabled(m_actions.testFlag(Edit));
    ui->btnDelete->setVisible(m_actions.testFlag(Delete));

    ui->wgtAction->setVariables(m_page->model()->variableNames());

    if (m_page && m_slot.type() != GGConnectionSlot::NoConnection) {
        setEnabled(true);
        GGConnection *c = slot.getExistingConnection(page);
        if (c) {
            if (c->destination()->name().isEmpty()) {
                ui->lblConnectedPage->setText(tr("{Page %1}").arg(c->destinationId()));
            } else {
                ui->lblConnectedPage->setText(c->destination()->name());
            }
        }
        switch (m_slot.type()) {
        case GGConnectionSlot::NoConnection:
            qFatal("GGConnectionEditorWidget::setConnection: NoConnection. Can never reach this");
            break;
        case GGConnectionSlot::StartConnection:
            ui->lblType->setText(tr("Start Connection"));
            break;
        case GGConnectionSlot::TrueConnection:
            ui->lblType->setText(tr("True Connection"));
            break;
        case GGConnectionSlot::FalseConnection:
            ui->lblType->setText(tr("False Connection"));
            break;
        case GGConnectionSlot::ActionConnection:
            ui->lblType->setText(tr("Action Connection"));
            ui->txtCaption->setText(GG::as<GGActionPage>(page)->actionLink().name());
            ui->wgtAction->setAction(GG::as<GGActionPage>(page)->actionLink().action());
            break;
        case GGConnectionSlot::DecisionConnection:
            if (m_fields.testFlag(Number)) ui->lblNumber->setVisible(true);
            ui->lblNumber->setText(QString("#%1").arg(m_slot.index()+1));
            ui->lblType->setText("Decision");
            ui->txtCaption->setText(GG::as<GGDecisionPage>(page)->getDecisionLinks().value(m_slot.index()).name());
            ui->wgtAction->setAction(GG::as<GGDecisionPage>(page)->getDecisionLinks().value(m_slot.index()).action());
            break;
        case GGConnectionSlot::MappedConnection:
            if (m_fields.testFlag(Number)) ui->lblNumber->setVisible(true);
            ui->lblNumber->setText(QString("#%1").arg(m_slot.index()+1));
            ui->lblType->setText("Mapped");
            ui->txtCaption->setText(GG::as<GGMappedContentPage>(page)->getLinkMap().value(m_slot.index()).link().name());
            ui->wgtAction->setAction(GG::as<GGMappedContentPage>(page)->getLinkMap().value(m_slot.index()).link().action());
            break;
        case GGConnectionSlot::AllConnections:
            // Sentinel to disable Warning
            break;
        }
    } else {
        setEnabled(false);
    }
}

//void GGConnectionEditorWidget::setCheckable(bool checkable)
//{
//    ui->btnActivate->setCheckable(checkable);
//}

//void GGConnectionEditorWidget::setEditable(bool editable)
//{
//    m_editable = editable;
//    setConnection(m_page, m_slot);
//}

void GGConnectionEditorWidget::setChecked(bool checked)
{
    if (ui->btnActivate->isCheckable()) {
        ui->btnActivate->setChecked(checked);
    }
}

//void GGConnectionEditorWidget::setDeletable(bool canDelete)
//{
//    ui->btnDelete->setVisible(canDelete);
//}

void GGConnectionEditorWidget::activateClicked()
{
    if (!ui->btnActivate->isCheckable()) {
        emit this->activated(m_page, m_slot);
    }
}

void GGConnectionEditorWidget::activateToggled(bool toggled)
{
    if (ui->btnActivate->isCheckable()) {
        emit this->toggled(toggled, m_page, m_slot);
    }
}

void GGConnectionEditorWidget::captionEdited()
{
    if (m_actions.testFlag(Edit)) {
        emit updateCaption(m_page, m_slot, ui->txtCaption->text());
    }
}

void GGConnectionEditorWidget::actionUpdated()
{
    emit updateAction(m_page, m_slot, ui->wgtAction->getAction());
}

void GGConnectionEditorWidget::deleteMe()
{
    emit deleteConnection(m_page, m_slot);
}

void GGConnectionEditorWidget::numberHoverEnter()
{
    if (m_actions.testFlag(Hover)) {
        static_cast<QLabel*> (sender())->setStyleSheet(m_numberHoverStyle);
        emit hoverEnter(m_page, m_slot);
    }
}

void GGConnectionEditorWidget::numberHoverLeave()
{
    if (m_actions.testFlag(Hover)) {
        static_cast<QLabel*> (sender())->setStyleSheet(m_numberBaseStyle);
        emit hoverLeave(m_page, m_slot);
    }
}
