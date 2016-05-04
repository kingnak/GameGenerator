#include "ggmediatreemodel.h"
#include <model/ggmediamanager.h>
#include <model/ggscenemediamanager.h>

const QString GGMediaTreeModel::ENTRY_DIR("DIR");
const QString GGMediaTreeModel::ENTRY_FILE("FILE");

class MediaTreeItem
{
public:
    explicit MediaTreeItem(QString path, QString disp, QString type, QString id = QString::null, MediaTreeItem *parentItem = 0)
        : m_path(path), m_disp(disp), m_type(type), m_id(id), m_parentItem(parentItem)
    {
    }

    ~MediaTreeItem() { qDeleteAll(m_childItems); }

    void appendChild(MediaTreeItem *child) { m_childItems << child; }

    MediaTreeItem *child(int row) { return m_childItems.value(row); }
    int childCount() const { return m_childItems.size(); }
    int columnCount() const { return 1; }
    QVariant data(int col, int role) const {
        if (col == 0) {
            if (role == Qt::DisplayRole) {
                return m_disp;
            } else if (role == GGMediaTreeModel::PathRole) {
                return m_path;
            } else if (role == GGMediaTreeModel::TypeRole) {
                return m_type;
            } else if (role == GGMediaTreeModel::IdRole) {
                return m_id;
            }
        }
        return QVariant();
    }
    int row() const { return m_parentItem ? m_parentItem->m_childItems.indexOf(const_cast<MediaTreeItem*>(this)) : 0; }
    MediaTreeItem *parentItem() { return m_parentItem; }

private:
    QList<MediaTreeItem*> m_childItems;
    QString m_path;
    QString m_disp;
    QString m_type;
    QString m_id;
    MediaTreeItem *m_parentItem;
};

GGMediaTreeModel::GGMediaTreeModel(GGMediaManager *mgm, QObject *parent)
    : QAbstractItemModel(parent),
      m_manager(mgm),
      m_root(NULL)
{
    reload();
}

GGMediaTreeModel::~GGMediaTreeModel()
{
    delete m_root;
}

QVariant GGMediaTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    MediaTreeItem *item = static_cast<MediaTreeItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

QVariant GGMediaTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Path";
    }
    return QVariant();
}

int GGMediaTreeModel::rowCount(const QModelIndex &parent) const
{
    MediaTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<MediaTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int GGMediaTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex GGMediaTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    MediaTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<MediaTreeItem*>(parent.internalPointer());

    MediaTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}

QModelIndex GGMediaTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    MediaTreeItem *childItem = static_cast<MediaTreeItem*>(child.internalPointer());
    MediaTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

void GGMediaTreeModel::reload()
{
    this->beginResetModel();
    delete m_root;
    m_root = new MediaTreeItem(m_manager->baseDir().absolutePath(), "Media", "DIR", NULL);

    m_manager->synchronize();
    QStringList lst = m_manager->allMediaWithDirs();
    qSort(lst);

    QMap<QString, MediaTreeItem*> items;

    foreach (QString path, lst) {
        path = QDir::fromNativeSeparators(path);
        QStringList parts = path.split('/');
        QString line;
        MediaTreeItem *parent = m_root;
        for (int i = 0; i < parts.size(); ++i) {
            line += parts[i];
            if (!items.contains(line)) {
                QString filePath = m_manager->baseDir().absoluteFilePath(line);
                QString type = QFileInfo(filePath).isDir() ? ENTRY_DIR : ENTRY_FILE;
                QString disp = m_manager->getDisplayString(parts[i], i);
                QString id = m_manager->getIdForFilePath(line);
                MediaTreeItem *i = new MediaTreeItem(filePath, disp, type, id, parent);
                items[line] = i;
                parent->appendChild(i);
                parent = i;
            } else {
                parent = items[line];
            }
            line += "/";
        }
    }
    this->endResetModel();
}

QStringList GGMediaTreeModel::verify()
{
    return m_manager->verify();
}

QStringList GGMediaTreeModel::cleanUp()
{
    QStringList ret = m_manager->cleanUp();
    if (!ret.isEmpty()) {
        reload();
    }
    return ret;
}
