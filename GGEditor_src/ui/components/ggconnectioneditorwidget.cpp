#include "ggconnectioneditorwidget.h"
#include "ui_ggconnectioneditorwidget.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGConnectionEditorWidget::GGConnectionEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGConnectionEditorWidget),
    m_page(NULL),
    m_slot(GGConnectionSlot::NoConnection),
    m_fields(All),
    m_editable(true)
{
    ui->setupUi(this);
    ui->btnDelete->setVisible(false);
    connect(ui->wgtAction, SIGNAL(actionUpdated(GGAction)), this, SLOT(actionUpdated()));
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

    if (slot.isLink()) {
        if (m_fields.testFlag(Action)) ui->wgtAction->setVisible(true);
        if (m_fields.testFlag(Caption)) ui->txtCaption->setVisible(true);
    }

    ui->txtCaption->setEnabled(m_editable);
    ui->wgtAction->setEditable(m_editable);
    ui->btnDelete->setEnabled(m_editable);

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
            qFatal("Can never reach this");
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
        }
    } else {
        setEnabled(false);
    }
}

void GGConnectionEditorWidget::setCheckable(bool checkable)
{
    ui->btnActivate->setCheckable(checkable);
}

void GGConnectionEditorWidget::setEditable(bool editable)
{
    m_editable = editable;
    setConnection(m_page, m_slot);
}

void GGConnectionEditorWidget::setChecked(bool checked)
{
    if (ui->btnActivate->isCheckable()) {
        ui->btnActivate->setChecked(checked);
    }
}

void GGConnectionEditorWidget::setDeletable(bool canDelete)
{
    ui->btnDelete->setVisible(canDelete);
}

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
    if (m_editable) {
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
