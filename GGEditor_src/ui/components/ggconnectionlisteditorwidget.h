#ifndef GGCONNECTIONLISTEDITORWIDGET_H
#define GGCONNECTIONLISTEDITORWIDGET_H

#include <QScrollArea>
#include "ggconnectioneditorwidget.h"
#include <model/ggconnectionslot.h>
#include <QList>

class GGPage;

namespace Ui {
class GGConnectionListEditorWidget;
}

class GGConnectionListEditorWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit GGConnectionListEditorWidget(QWidget *parent = 0);
    ~GGConnectionListEditorWidget();

    GGConnectionSlot selectedSlot();

public slots:
    void setConnections(GGPage *page, QList<GGConnectionSlot> connections);
    void cleanConnections();

    void setFields(int f);
    void setActions(int a);

signals:
    void deleteConnection(GGPage *page, GGConnectionSlot slot);
    void connectConnection(GGPage *page, GGConnectionSlot slot);
    void selectedConnection(GGPage *page, GGConnectionSlot slot);
    void selectedConnection(int idx);
    void hoverEnteredConnection(GGPage *page, GGConnectionSlot slot);
    void hoverLeftConnection(GGPage *page, GGConnectionSlot slot);

private slots:
    void handleToggle();

private:
    Ui::GGConnectionListEditorWidget *ui;
    GGPage *m_page;
    GGConnectionEditorWidget::Fields m_fields;
    GGConnectionEditorWidget::ConnectionActions m_actions;
    int m_curSelected;
};

#endif // GGCONNECTIONLISTEDITORWIDGET_H
