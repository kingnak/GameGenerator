#include "ggchoseconnectionslotdlg.h"
#include "ui_ggchoseconnectionslotdlg.h"
#include "../components/ggconnectioneditorwidget.h"
#include <model/ggconnectionslot.h>
#include <QLayoutItem>
#include <QPushButton>

GGChoseConnectionSlotDlg::GGChoseConnectionSlotDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGChoseConnectionSlotDlg),
    m_page(NULL)
{
    ui->setupUi(this);
    ui->wgtConnections->setActions(GGConnectionEditorWidget::Select);
    ui->wgtConnections->setFields(GGConnectionEditorWidget::Type | GGConnectionEditorWidget::Page | GGConnectionEditorWidget::Number | GGConnectionEditorWidget::Caption);
    connect(ui->wgtConnections, SIGNAL(selectedConnection(int)), this, SLOT(toggledOne(int)));
}

GGChoseConnectionSlotDlg::~GGChoseConnectionSlotDlg()
{
    delete ui;
}

GGPage *GGChoseConnectionSlotDlg::page()
{
    return m_page;
}

GGConnectionSlot GGChoseConnectionSlotDlg::selectedSlot()
{
    return ui->wgtConnections->selectedSlot();
}

void GGChoseConnectionSlotDlg::setConnectionSlots(GGPage *source)
{
    m_page = source;
    QList<GGConnectionSlot> slts = GGConnectionSlot::enumerateConnections(source);
    ui->wgtConnections->setConnections(m_page, slts);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void GGChoseConnectionSlotDlg::toggledOne(int idx)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(idx >= 0);
}
