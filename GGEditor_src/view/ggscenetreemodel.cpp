#include "ggscenetreemodel.h"
#include <model/ggeditmodel.h>
#include <model/ggscene.h>

class TreeItem
{
public:
    explicit TreeItem(GGScene *scene, TreeItem *parentItem = 0)
        : m_scene(scene), m_parentItem(parentItem)
    {
    }

    ~TreeItem() { qDeleteAll(m_childItems); }

    void appendChild(TreeItem *child) { m_childItems << child; }

    TreeItem *child(int row) { return m_childItems.value(row); }
    int childCount() const { return m_childItems.size(); }
    int columnCount() const { return 1; }
    QVariant data(int col, int role) const {
        if (col == 0) {
            if (role == Qt::DisplayRole) {
                return m_scene->name();
            } else if (role == GGSceneTreeModel::SceneIdRole) {
                return m_scene->id();
            }
        }
        return QVariant();
    }
    int row() const { return m_parentItem ? m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this)) : 0; }
    TreeItem *parentItem() { return m_parentItem; }

private:
    QList<TreeItem*> m_childItems;
    GGScene *m_scene;
    TreeItem *m_parentItem;
};


GGSceneTreeModel::GGSceneTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_model(NULL),
      m_root(NULL)
{

}

GGSceneTreeModel::~GGSceneTreeModel()
{
    delete m_root;
}

QVariant GGSceneTreeModel::data(const QModelIndex &index, int role) const
{
    if (!m_model) return QVariant();
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column(), role);
}

QVariant GGSceneTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return "Name";
    }
    return QVariant();
}

int GGSceneTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!m_model) return 0;
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int GGSceneTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex GGSceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_model) return QModelIndex();
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex GGSceneTreeModel::parent(const QModelIndex &child) const
{
    if (!m_model || !child.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

void GGSceneTreeModel::setModel(GGEditModel *model)
{
    if (m_model) {
        disconnect(m_model, SIGNAL(sceneRegistered(GGScene*)), this, SLOT(updModel()));
        disconnect(m_model, SIGNAL(sceneUnregistered(GG::SceneID,GGScene*)), this, SLOT(updModel()));
    }
    m_model = model;
    if (m_model) {
        connect(m_model, SIGNAL(sceneRegistered(GGScene*)), this, SLOT(updModel()));
        connect(m_model, SIGNAL(sceneUnregistered(GG::SceneID,GGScene*)), this, SLOT(updModel()));
    }
}

void GGSceneTreeModel::updModel()
{
    this->beginResetModel();
    delete m_root;
    if (m_model) {
        m_root = new TreeItem(NULL, NULL);
        for (int i = 0; i < m_model->getScenes().count(); ++i) {
            TreeItem *c = new TreeItem(m_model->getScenes()[i], m_root);
            m_root->appendChild(c);
        }
    }
    this->endResetModel();
}
