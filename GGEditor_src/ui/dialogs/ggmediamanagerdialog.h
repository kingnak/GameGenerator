#ifndef GGMEDIAMANAGERDIALOG_H
#define GGMEDIAMANAGERDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class GGMediaManagerDialog;
}

class GGMediaManager;

class MediaTreeItem;

class GGMediaTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum {
        PathRole = Qt::UserRole
    };
    GGMediaTreeModel(GGMediaManager *mgm, QObject *parent = 0);
    ~GGMediaTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

public slots:
    void reload();

private:
    GGMediaManager *m_manager;
    MediaTreeItem *m_root;
};

class GGMediaManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGMediaManagerDialog(GGMediaManager *mgm, QWidget *parent = 0);
    ~GGMediaManagerDialog();

public slots:
    void refresh();

private:
    Ui::GGMediaManagerDialog *ui;

    GGMediaTreeModel *m_tree;
};

#endif // GGMEDIAMANAGERDIALOG_H
