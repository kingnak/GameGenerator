#ifndef GGCONNECTIONEDITORWIDGET_H
#define GGCONNECTIONEDITORWIDGET_H

#include <QWidget>
#include <model/ggconnectionslot.h>
#include <model/ggaction.h>

namespace Ui {
class GGConnectionEditorWidget;
}

class GGConnectionEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGConnectionEditorWidget(QWidget *parent = 0);
    ~GGConnectionEditorWidget();

    enum Field {
        Type    = 0x0001,
        Caption = 0x0002,
        Action  = 0x0004,
        Page    = 0x0008,
        Number  = 0x0010,
        AllFields   = Type|Caption|Action|Page|Number
    };
    Q_DECLARE_FLAGS(Fields, Field)
    enum ConnectionAction {
        Delete  = 0x0001,
        Connect = 0x0002,
        Select  = 0x0004,
        Edit    = 0x0008,
        Hover   = 0x0010,
        DefaultActions  = Delete | Connect | Edit
    };
    Q_DECLARE_FLAGS(ConnectionActions, ConnectionAction)

    void setFields(int f);
    void setActions(int a);

    GGConnectionSlot slot();
    GGPage *page();
    GGAction getAction();
    QString getCaption();

public slots:
    void setConnection(GGPage *page, GGConnectionSlot slot);
    //void setCheckable(bool checkable);
    //void setEditable(bool editable);
    void setChecked(bool checked);
    //void setDeletable(bool canDelete);

signals:
    void activated(GGPage *page, GGConnectionSlot slot);
    void toggled(bool toggle, GGPage *page, GGConnectionSlot slot);
    void updateCaption(GGPage *page, GGConnectionSlot slot, QString caption);
    void updateAction(GGPage *page, GGConnectionSlot slot, GGAction action);
    void deleteConnection(GGPage *page, GGConnectionSlot slot);
    void hoverEnter(GGPage *page, GGConnectionSlot slot);
    void hoverLeave(GGPage *page, GGConnectionSlot slot);

private slots:
    void activateClicked();
    void activateToggled(bool toggled);
    void captionEdited();
    void actionUpdated();
    void deleteMe();
    void numberHoverEnter();
    void numberHoverLeave();

private:
    Ui::GGConnectionEditorWidget *ui;
    GGPage *m_page;
    GGConnectionSlot m_slot;
    //bool m_editable;
    Fields m_fields;
    ConnectionActions m_actions;
    QString m_numberBaseStyle;
    QString m_numberHoverStyle;
};

#endif // GGCONNECTIONEDITORWIDGET_H
