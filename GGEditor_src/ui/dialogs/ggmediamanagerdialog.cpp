#include "ggmediamanagerdialog.h"
#include "ui_ggmediamanagerdialog.h"
#include <QAbstractItemModel>
#include <model/ggmediamanager.h>
#include <model/ggscenemediamanager.h>

class MediaTreeItem
{
public:
    explicit MediaTreeItem(QString path, QString disp, MediaTreeItem *parentItem = 0)
        : m_path(path), m_disp(disp), m_parentItem(parentItem)
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
    delete m_root;
    m_root = new MediaTreeItem(m_manager->baseDir().absolutePath(), "Media", NULL);

    QStringList lst = m_manager->allMedia();
    qSort(lst);

    QMap<QString, MediaTreeItem*> items;

    foreach (QString path, lst) {
        path = QDir::fromNativeSeparators(path);
        QStringList parts = path.split('/');
        QString line;
        MediaTreeItem *parent = m_root;
        foreach (QString p, parts) {
            line += "/" + p;
            if (!items.contains(line)) {
                MediaTreeItem *i = new MediaTreeItem(m_manager->baseDir().absoluteFilePath(line), p, parent);
                items[line] = i;
                parent->appendChild(i);
                parent = i;
            } else {
                parent = items[line];
            }
        }
    }
}

//////////////////////

GGMediaManagerDialog::GGMediaManagerDialog(GGMediaManager *mgm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGMediaManagerDialog)
{
    ui->setupUi(this);
    m_tree = new GGMediaTreeModel(mgm, this);
    ui->treeFolders->setModel(m_tree);
    refresh();
}

GGMediaManagerDialog::~GGMediaManagerDialog()
{
    delete ui;
}

void GGMediaManagerDialog::refresh()
{
    m_tree->reload();
}
