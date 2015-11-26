#include "ggchoseconnectionslotdlg.h"
#include "ui_ggchoseconnectionslotdlg.h"
#include "../components/ggconnectioneditorwidget.h"
#include <model/ggconnectionslot.h>
#include <QLayoutItem>
#include <QPushButton>

GGChoseConnectionSlotDlg::GGChoseConnectionSlotDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGChoseConnectionSlotDlg),
    m_page(NULL),
    m_curSelected(-1)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(new QVBoxLayout);
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
    QList<GGConnectionEditorWidget*> lst = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
    if (m_curSelected >= 0) {
        return lst.value(m_curSelected)->slot();
    }
    return GGConnectionSlot::NoConnection;
}

void GGChoseConnectionSlotDlg::setConnectionSlots(GGPage *source)
{
    m_curSelected = -1;
    qDeleteAll(ui->scrollAreaWidgetContents->findChildren<QWidget*>());
    m_page = source;
    QList<GGConnectionSlot> slts = GGConnectionSlot::enumerateConnections(source);
    foreach (GGConnectionSlot s, slts) {
        GGConnectionEditorWidget *w = new GGConnectionEditorWidget;
        w->setConnection(m_page, s);
        w->setCheckable(true);
        w->setEditable(false);
        w->setFields(GGConnectionEditorWidget::Type | GGConnectionEditorWidget::Page | GGConnectionEditorWidget::Number | GGConnectionEditorWidget::Caption);
        connect(w, SIGNAL(toggled(bool,GGPage*,GGConnectionSlot)), this, SLOT(toggledOne()));
        ui->scrollAreaWidgetContents->layout()->addWidget(w);
    }
    ui->scrollAreaWidgetContents->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_curSelected >= 0);
}

void GGChoseConnectionSlotDlg::toggledOne()
{
    QList<GGConnectionEditorWidget*> lst = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
    if (m_curSelected >= 0) {
        lst.value(m_curSelected)->setChecked(false);
    }
    m_curSelected = lst.indexOf(static_cast<GGConnectionEditorWidget*> (sender()));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(m_curSelected >= 0);
}
