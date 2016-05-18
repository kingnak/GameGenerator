#include "ggconnectionlisteditorwidget.h"
#include "ui_ggconnectionlisteditorwidget.h"
#include <QVBoxLayout>

GGConnectionListEditorWidget::GGConnectionListEditorWidget(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::GGConnectionListEditorWidget),
    m_page(NULL),
    m_fields(GGConnectionEditorWidget::AllFields),
    m_actions(GGConnectionEditorWidget::DefaultActions),
    m_curSelected(-1)
{
    ui->setupUi(this);
    QVBoxLayout *l = new QVBoxLayout;
    ui->editorContainer->setLayout(l);
    l->addStretch(1);

    ensureEditors(MIN_EDITORS);
}

GGConnectionListEditorWidget::~GGConnectionListEditorWidget()
{
    delete ui;
}

GGConnectionSlot GGConnectionListEditorWidget::selectedSlot()
{
    if (m_actions.testFlag(GGConnectionEditorWidget::Select) && m_curSelected >= 0) {
        QList<GGConnectionEditorWidget *> wgts = ui->editorContainer->findChildren<GGConnectionEditorWidget*>();
        return wgts[m_curSelected]->slot();
    }
    return GGConnectionSlot::NoConnection;
}

void GGConnectionListEditorWidget::setConnections(GGPage *page, QList<GGConnectionSlot> connections)
{
    m_page = page;

    ensureEditors(connections.size());
    if (!page) return;

    for (int i = 0; i < connections.size(); ++i) {
        GGConnectionEditorWidget *w = m_editors[i];
        w->setConnection(page, connections[i]);
        w->setFields(m_fields);
        w->setActions(m_actions);
        w->setVisible(true);
    }
    ui->scrollArea->ensureVisible(0, 0);
}

void GGConnectionListEditorWidget::cleanConnections()
{
    ensureEditors(MIN_EDITORS);
    m_curSelected = -1;
}

void GGConnectionListEditorWidget::setFields(int f)
{
    if (m_fields != f) {
        m_fields = static_cast<GGConnectionEditorWidget::Fields> (f);
        QList<GGConnectionEditorWidget*> wgts = ui->editorContainer->findChildren<GGConnectionEditorWidget*>();
        foreach (GGConnectionEditorWidget *w, wgts) {
            w->setFields(f);
        }
    }
}

void GGConnectionListEditorWidget::setActions(int a)
{
    if (m_actions != a) {
        m_actions = static_cast<GGConnectionEditorWidget::ConnectionActions> (a);
        QList<GGConnectionEditorWidget*> wgts = ui->editorContainer->findChildren<GGConnectionEditorWidget*>();
        foreach (GGConnectionEditorWidget *w, wgts) {
            w->setActions(a);
        }
    }
}

void GGConnectionListEditorWidget::handleToggle()
{
    QList<GGConnectionEditorWidget *> wgts = ui->editorContainer->findChildren<GGConnectionEditorWidget*>();
    if (m_curSelected >= 0) {
        wgts[m_curSelected]->setChecked(false);
    }
    m_curSelected = wgts.indexOf(static_cast<GGConnectionEditorWidget*>(sender()));
    if (m_curSelected >= 0) {
        emit selectedConnection(m_page, wgts[m_curSelected]->slot());
        emit selectedConnection(m_curSelected);
    }
}

void GGConnectionListEditorWidget::ensureEditors(int ct)
{
    if (ct == m_editors.size()) return;

    QVBoxLayout *l = static_cast<QVBoxLayout*> (ui->editorContainer->layout());

    if (ct > m_editors.size()) {
        // Need more editors. create
        for (int i = m_editors.size(); i < ct; ++i) {
            GGConnectionEditorWidget *w = new GGConnectionEditorWidget(ui->editorContainer);
            w->setVisible(false);
            l->insertWidget(l->count()-1, w);
            connect(w, SIGNAL(updateCaption(GGPage*,GGConnectionSlot,QString)), this, SIGNAL(updateLinkCaption(GGPage*,GGConnectionSlot,QString)));
            connect(w, SIGNAL(updateAction(GGPage*,GGConnectionSlot,GGAction)), this, SIGNAL(updateLinkAction(GGPage*,GGConnectionSlot,GGAction)));
            connect(w, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)), this, SIGNAL(deleteConnection(GGPage*,GGConnectionSlot)));
            connect(w, SIGNAL(activated(GGPage*,GGConnectionSlot)), this, SIGNAL(connectConnection(GGPage*,GGConnectionSlot)));
            connect(w, SIGNAL(toggled(bool,GGPage*,GGConnectionSlot)), this, SLOT(handleToggle()));
            connect(w, SIGNAL(hoverEnter(GGPage*,GGConnectionSlot)), this, SIGNAL(hoverEnteredConnection(GGPage*,GGConnectionSlot)));
            connect(w, SIGNAL(hoverLeave(GGPage*,GGConnectionSlot)), this, SIGNAL(hoverLeftConnection(GGPage*,GGConnectionSlot)));
            m_editors << w;
        }
    } else {

        // Too many editors. Hide unneeded...
        for (int i = qMax(0, ct-1); i < MIN_EDITORS; ++i) {
            m_editors[i]->setVisible(false);
        }
        // Fix for mingw: cannot use MIN_EDITORS in qMax...
        int tmp = MIN_EDITORS;
        ct = qMax(ct, tmp);
        // ... and delete additional
        if (m_editors.size() > ct) {
            for (int i = ct; i < m_editors.size(); ++i) {
                GGConnectionEditorWidget *w = m_editors[i];
                l->removeWidget(w);
                w->deleteLater();
            }
            m_editors.erase(m_editors.begin()+ct, m_editors.end());
        }
    }
}
