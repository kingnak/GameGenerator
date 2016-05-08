#ifndef GGMEDIATREEMODEL_H
#define GGMEDIATREEMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

class GGSceneMediaManager;

class MediaTreeItem;

class GGMediaTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum {
        PathRole = Qt::UserRole,
        TypeRole,
        IdRole,
        SceneRole,
        MediaTypeRole
    };

    static const QString ENTRY_DIR;
    static const QString ENTRY_FILE;

    GGMediaTreeModel(GGSceneMediaManager *mgm, QObject *parent = 0);
    ~GGMediaTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    GGSceneMediaManager *manager();
    QPixmap loadFile(const QString &id);

public slots:
    void reload();
    QStringList verify();
    QStringList cleanUp();

private:
    GGSceneMediaManager *m_manager;
    MediaTreeItem *m_root;
};


#endif // GGMEDIATREEMODEL_H
