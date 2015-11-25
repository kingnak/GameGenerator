#include "ggconnectioneditorwidget.h"
#include "ui_ggconnectioneditorwidget.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGConnectionEditorWidget::GGConnectionEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGConnectionEditorWidget),
    m_page(NULL),
    m_slot(GGConnectionSlot::NoConnection)
{
    ui->setupUi(this);
}

GGConnectionEditorWidget::~GGConnectionEditorWidget()
{
    delete ui;
}

GGConnectionSlot GGConnectionEditorWidget::slot()
{
    return m_slot;
}

GGPage *GGConnectionEditorWidget::page()
{
    return m_page;
}

void GGConnectionEditorWidget::setConnection(GGPage *page, GGConnectionSlot slot)
{
    m_page = page;
    m_slot = slot;
    ui->txtCaption->setEnabled(false);
    ui->txtCaption->setText("");
    ui->lblNumber->setVisible(false);
    ui->lblConnectedPage->setText("");
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
            ui->txtCaption->setEnabled(m_editable);
            ui->txtCaption->setText(GG::as<GGActionPage>(page)->actionLink().name());
            break;
        case GGConnectionSlot::DecisionConnection:
            ui->lblNumber->setVisible(true);
            ui->lblNumber->setText(QString("#%1").arg(m_slot.index()));
            ui->lblType->setText("Decision");
            ui->txtCaption->setEnabled(m_editable);
            ui->txtCaption->setText(GG::as<GGDecisionPage>(page)->getDecisionLinks().value(m_slot.index()).name());
            break;
        case GGConnectionSlot::MappedConnection:
            ui->lblNumber->setVisible(true);
            ui->lblNumber->setText(QString("#%1").arg(m_slot.index()));
            ui->lblType->setText("Mapped");
            ui->txtCaption->setEnabled(m_editable);
            ui->txtCaption->setText(GG::as<GGMappedContentPage>(page)->getLinkMap().value(m_slot.index()).link().name());
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

void GGConnectionEditorWidget::activateClicked()
{
    if (!ui->btnActivate->isCheckable()) {
        activated(m_page, m_slot);
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
