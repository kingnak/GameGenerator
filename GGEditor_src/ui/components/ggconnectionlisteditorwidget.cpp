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
    viewport()->setLayout(new QVBoxLayout);
}

GGConnectionListEditorWidget::~GGConnectionListEditorWidget()
{
    delete ui;
}

GGConnectionSlot GGConnectionListEditorWidget::selectedSlot()
{
    if (m_actions.testFlag(GGConnectionEditorWidget::Select) && m_curSelected >= 0) {
        QList<GGConnectionEditorWidget *> wgts = viewport()->findChildren<GGConnectionEditorWidget*>();
        return wgts[m_curSelected]->slot();
    }
    return GGConnectionSlot::NoConnection;
}

void GGConnectionListEditorWidget::setConnections(GGPage *page, QList<GGConnectionSlot> connections)
{
    m_page = page;

    cleanConnections();
    if (!page) return;

    for (int i = 0; i < connections.size(); ++i) {
        GGConnectionEditorWidget *w = new GGConnectionEditorWidget;
        w->setConnection(page, connections[i]);
        w->setFields(m_fields);
        w->setActions(m_actions);
        connect(w, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)));
        connect(w, SIGNAL(activated(GGPage*,GGConnectionSlot)), this, SIGNAL(connectConnection(GGPage*,GGConnectionSlot)));
        connect(w, SIGNAL(toggled(bool,GGPage*,GGConnectionSlot)), this, SLOT(handleToggle()));
        viewport()->layout()->addWidget(w);
    }
    viewport()->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void GGConnectionListEditorWidget::cleanConnections()
{
    while (viewport()->layout()->count()) {
        delete viewport()->layout()->takeAt(0);
    }
    QList<GGConnectionEditorWidget*> l = viewport()->findChildren<GGConnectionEditorWidget*>();
    qDeleteAll(l);
    m_curSelected = -1;
}

void GGConnectionListEditorWidget::setFields(int f)
{
    if (m_fields != f) {
        m_fields = static_cast<GGConnectionEditorWidget::Fields> (f);
        QList<GGConnectionEditorWidget*> wgts = viewport()->findChildren<GGConnectionEditorWidget*>();
        foreach (GGConnectionEditorWidget *w, wgts) {
            w->setFields(f);
        }
    }
}

void GGConnectionListEditorWidget::setActions(int a)
{
    if (m_actions != a) {
        m_actions = static_cast<GGConnectionEditorWidget::ConnectionActions> (a);
        QList<GGConnectionEditorWidget*> wgts = viewport()->findChildren<GGConnectionEditorWidget*>();
        foreach (GGConnectionEditorWidget *w, wgts) {
            w->setActions(a);
        }
    }
}

void GGConnectionListEditorWidget::handleToggle()
{
    QList<GGConnectionEditorWidget *> wgts = viewport()->findChildren<GGConnectionEditorWidget*>();
    if (m_curSelected >= 0) {
        wgts[m_curSelected]->setChecked(false);
    }
    m_curSelected = wgts.indexOf(static_cast<GGConnectionEditorWidget*>(sender()));
    if (m_curSelected >= 0) {
        emit selectedConnection(m_page, wgts[m_curSelected]->slot());
        emit selectedConnection(m_curSelected);
    }
}
