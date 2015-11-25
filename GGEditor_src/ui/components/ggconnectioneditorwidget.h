#ifndef GGCONNECTIONEDITORWIDGET_H
#define GGCONNECTIONEDITORWIDGET_H

#include <QWidget>
#include <model/ggconnectionslot.h>

namespace Ui {
class GGConnectionEditorWidget;
}

class GGConnectionEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGConnectionEditorWidget(QWidget *parent = 0);
    ~GGConnectionEditorWidget();

    GGConnectionSlot slot();
    GGPage *page();

public slots:
    void setConnection(GGPage *page, GGConnectionSlot slot);
    void setCheckable(bool checkable);
    void setEditable(bool editable);
    void setChecked(bool checked);

signals:
    void activated(GGPage *page, GGConnectionSlot slot);
    void toggled(bool toggle, GGPage *page, GGConnectionSlot slot);
    void updateCaption(GGPage *page, GGConnectionSlot slot, QString caption);

private slots:
    void activateClicked();
    void activateToggled(bool toggled);
    void captionEdited();

private:
    Ui::GGConnectionEditorWidget *ui;
    GGPage *m_page;
    GGConnectionSlot m_slot;
    bool m_editable;
};

#endif // GGCONNECTIONEDITORWIDGET_H
