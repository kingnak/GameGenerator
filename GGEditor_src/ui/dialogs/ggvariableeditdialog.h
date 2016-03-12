#ifndef GGVARIABLEEDITDIALOG_H
#define GGVARIABLEEDITDIALOG_H

#include <QDialog>
#include <QAbstractListModel>
#include <QStringList>

class GGEditModel;

namespace Ui {
class GGVariableEditDialog;
}

class GGVariableEditModel : public QAbstractListModel
{
    Q_OBJECT
public:
    GGVariableEditModel(GGEditModel *model, QWidget *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void addVariable();
    void removeVariable(int idx);

    GGEditModel *dataModel() { return m_model; }

private slots:
    void reloadData();

private:
    GGEditModel *m_model;
    QStringList m_sortedList;
};

////////////////////////////

class GGVariableEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGVariableEditDialog(QWidget *parent = 0);
    ~GGVariableEditDialog();

    void setModel(GGEditModel *model);

private slots:
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void setButtons();
    void showVarUsage();

private:
    Ui::GGVariableEditDialog *ui;
};

#endif // GGVARIABLEEDITDIALOG_H
