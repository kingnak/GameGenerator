#ifndef GGACTIONEDITORWIDGET_H
#define GGACTIONEDITORWIDGET_H

#include <QWidget>
#include <model/ggaction.h>

namespace Ui {
class GGActionEditorWidget;
}

class GGActionEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGActionEditorWidget(QWidget *parent = 0);
    ~GGActionEditorWidget();

    GGAction getAction();

public slots:
    void setAction(GGAction action);
    void setEditable(bool editable);

signals:
    void actionUpdated(GGAction action);

private slots:
    void on_btnEdit_clicked();
    void on_btnAbort_clicked();
    void on_btnOk_clicked();
    void on_cmbOp_currentIndexChanged(int);
    void updateAction();

private:
    Ui::GGActionEditorWidget *ui;
    GGAction m_action;
};

#endif // GGACTIONEDITORWIDGET_H