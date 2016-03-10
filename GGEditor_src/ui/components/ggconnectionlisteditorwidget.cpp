#include "ggconnectionlisteditorwidget.h"
#include "ui_ggconnectionlisteditorwidget.h"
#include <QVBoxLayout>

GGConnectionListEditorWidget::GGConnectionListEditorWidget(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::GGConnectionListEditorWidget),
    m_page(NULL),
    m_fields(GGConnectionEditorWidget::AllFields),
    m_actions(GGConnectionEditorWidget::AllActions)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(new QVBoxLayout);
}

GGConnectionListEditorWidget::~GGConnectionListEditorWidget()
{
    delete ui;
}

GGConnectionSlot GGConnectionListEditorWidget::selectedSlot()
{
    if (m_actions.testFlag(GGConnectionEditorWidget::Select) && m_curSelected >= 0) {
        QList<GGConnectionEditorWidget *> wgts = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
        return wgts[m_curSelected]->slot();
    }
    return GGConnectionSlot::NoConnection;
}

void GGConnectionListEditorWidget::setConnections(GGPage *page, QList<GGConnectionSlot> connections)
{
    m_page = page;

    cleanConnections();
    if (!page) return;

    QVBoxLayout *l = static_cast<QVBoxLayout*> (ui->scrollAreaWidgetContents->layout());
    for (int i = 0; i < connections.size(); ++i) {
        GGConnectionEditorWidget *w = new GGConnectionEditorWidget;
        w->setConnection(page, connections[i]);
        w->setFields(m_fields);
        w->setActions(m_actions);
        //w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        connect(w, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)));
        connect(w, SIGNAL(activated(GGPage*,GGConnectionSlot)), this, SIGNAL(connectConnection(GGPage*,GGConnectionSlot)));
        connect(w, SIGNAL(toggled(bool,GGPage*,GGConnectionSlot)), this, SLOT(handleToggle()));
        l->addWidget(w, 0);
    }
    l->addStretch(1);
}

void GGConnectionListEditorWidget::cleanConnections()
{
    while (ui->scrollAreaWidgetContents->layout()->count()) {
        delete ui->scrollAreaWidgetContents->layout()->takeAt(0);
    }
    QList<GGConnectionEditorWidget*> l = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
    qDeleteAll(l);
    m_curSelected = -1;
}

void GGConnectionListEditorWidget::setFields(int f)
{
    if (m_fields != f) {
        m_fields = static_cast<GGConnectionEditorWidget::Fields> (f);
        QList<GGConnectionEditorWidget*> wgts = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
        foreach (GGConnectionEditorWidget *w, wgts) {
            w->setFields(f);
        }
    }
}

void GGConnectionListEditorWidget::setActions(int a)
{
    if (m_actions != a) {
        m_actions = static_cast<GGConnectionEditorWidget::ConnectionActions> (a);
        QList<GGConnectionEditorWidget*> wgts = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
        foreach (GGConnectionEditorWidget *w, wgts) {
            w->setActions(a);
        }
    }
}

void GGConnectionListEditorWidget::handleToggle()
{
    QList<GGConnectionEditorWidget *> wgts = ui->scrollAreaWidgetContents->findChildren<GGConnectionEditorWidget*>();
    if (m_curSelected >= 0) {
        wgts[m_curSelected]->setChecked(false);
    }
    m_curSelected = wgts.indexOf(static_cast<GGConnectionEditorWidget*>(sender()));
    if (m_curSelected >= 0) {
        emit selectedConnection(m_page, wgts[m_curSelected]->slot());
        emit selectedConnection(m_curSelected);
    }
}
