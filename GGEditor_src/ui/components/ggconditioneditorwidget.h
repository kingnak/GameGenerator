#ifndef GGCONDITIONEDITORWIDGET_H
#define GGCONDITIONEDITORWIDGET_H

#include <QWidget>
#include <model/ggcondition.h>

namespace Ui {
class GGConditionEditorWidget;
}

class GGStringListValidator;

class GGConditionEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GGConditionEditorWidget(QWidget *parent = 0);
    ~GGConditionEditorWidget();

    GGCondition getCondition();

public slots:
    void setVariables(QStringList vars);
    void setCondition(const GGCondition &cond);
    void setEditable(bool editable);

signals:
    void conditionUpdated(GGCondition cond);

private slots:
    void on_btnEdit_clicked();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_cmbOp_currentIndexChanged(int idx);

private:
    GGCondition buildCondition();

private:
    Ui::GGConditionEditorWidget *ui;

    GGCondition m_cond;
    QStringList m_varCache;
    GGStringListValidator *m_varValidator;
};

#endif // GGCONDITIONEDITORWIDGET_H
