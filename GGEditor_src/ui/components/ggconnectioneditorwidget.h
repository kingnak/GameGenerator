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
        All     = Type|Caption|Action|Page|Number
    };
    Q_DECLARE_FLAGS(Fields, Field)

    void setFields(int f);

    GGConnectionSlot slot();
    GGPage *page();
    GGAction getAction();
    QString getCaption();

public slots:
    void setConnection(GGPage *page, GGConnectionSlot slot);
    void setCheckable(bool checkable);
    void setEditable(bool editable);
    void setChecked(bool checked);
    void setDeletable(bool canDelete);

signals:
    void activated(GGPage *page, GGConnectionSlot slot);
    void toggled(bool toggle, GGPage *page, GGConnectionSlot slot);
    void updateCaption(GGPage *page, GGConnectionSlot slot, QString caption);
    void updateAction(GGPage *page, GGConnectionSlot slot, GGAction action);
    void deleteConnection(GGPage *page, GGConnectionSlot slot);

private slots:
    void activateClicked();
    void activateToggled(bool toggled);
    void captionEdited();
    void actionUpdated();
    void deleteMe();

private:
    Ui::GGConnectionEditorWidget *ui;
    GGPage *m_page;
    GGConnectionSlot m_slot;
    bool m_editable;
    Fields m_fields;
};

#endif // GGCONNECTIONEDITORWIDGET_H
