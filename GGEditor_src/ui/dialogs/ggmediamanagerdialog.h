#ifndef GGMEDIAMANAGERDIALOG_H
#define GGMEDIAMANAGERDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class GGMediaManagerDialog;
}

class GGSceneMediaManager;
class GGMediaTreeModel;

class GGMediaManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGMediaManagerDialog(GGSceneMediaManager *mgm, QWidget *parent = 0);
    ~GGMediaManagerDialog();

    QString getSelectedMediaId();

public slots:
    void refresh();
    void setSelectedDirectory(const QString &dir);
    void expandAll();
    void accept();
    int requestMedia();

private slots:
    void treeItemSelected(QModelIndex idx);
    void listItemSelected();
    void on_btnSynch_clicked();
    void on_btnVerify_clicked();
    void on_btnCleanup_clicked();
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void acceptMedia(QModelIndex idx);

private:
    void showDir(QModelIndex idx);

    QModelIndex selectedTreeIndex();
    QModelIndex selectedListIndex();

private:
    Ui::GGMediaManagerDialog *ui;

    GGMediaTreeModel *m_tree;
    QString m_initSelection;
    QString m_acceptedSelection;
    bool m_requestingMedia;
};

#endif // GGMEDIAMANAGERDIALOG_H
