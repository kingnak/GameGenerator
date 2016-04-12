#ifndef GGMEDIAMANAGERDIALOG_H
#define GGMEDIAMANAGERDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class GGMediaManagerDialog;
}

class GGMediaManager;
class GGMediaTreeModel;

class GGMediaManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGMediaManagerDialog(GGMediaManager *mgm, QWidget *parent = 0);
    ~GGMediaManagerDialog();

public slots:
    void refresh();

private slots:
    void treeItemSelected(QModelIndex idx);

private:
    void showDir(QModelIndex idx);

private:
    Ui::GGMediaManagerDialog *ui;

    GGMediaTreeModel *m_tree;
};

#endif // GGMEDIAMANAGERDIALOG_H
