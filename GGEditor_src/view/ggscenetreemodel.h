#ifndef GGSCENETREEMODEL_H
#define GGSCENETREEMODEL_H

#include <QAbstractItemModel>

class GGEditModel;
class TreeItem;

class GGSceneTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    GGSceneTreeModel(QObject *parent = NULL);
    ~GGSceneTreeModel();

    enum {
        SceneIdRole = Qt::UserRole
    };

    //Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

public slots:
    void setModel(GGEditModel *model);

private slots:
    void updModel();

private:
    void doUpdModel();

private:
    GGEditModel *m_model;
    TreeItem *m_root;
};

#endif // GGSCENETREEMODEL_H
