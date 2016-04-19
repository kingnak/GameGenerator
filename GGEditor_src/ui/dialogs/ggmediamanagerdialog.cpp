#include "ggmediamanagerdialog.h"
#include "ui_ggmediamanagerdialog.h"
#include <QSortFilterProxyModel>
#include <model/ggmediamanager.h>
#include <view/ggmediatreemodel.h>

class ListProxyModel : public QSortFilterProxyModel
{
public:
    ListProxyModel(QObject *parent = 0) : QSortFilterProxyModel(parent), m_invalidating(false) {}

    void setRootIndex(QModelIndex idx) {
        m_root = idx;
        m_invalidating = true;
        invalidateFilter();
        m_invalidating = false;
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
        if (m_invalidating) return true;

        // Requesting child of our root => visible if file
        if (source_parent == m_root) {
            QString t = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), GGMediaTreeModel::TypeRole).toString();
            return t == GGMediaTreeModel::ENTRY_FILE;
        }
        // Requesting a child of root's parent => visible if it is our parent
        if (source_parent == m_root.parent()) {
            if (source_row == m_root.row()) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

private:
    QModelIndex m_root;
    bool m_invalidating;
};

GGMediaManagerDialog::GGMediaManagerDialog(GGMediaManager *mgm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGMediaManagerDialog)
{
    ui->setupUi(this);
    m_tree = new GGMediaTreeModel(mgm, this);

    QSortFilterProxyModel *treeProxy = new QSortFilterProxyModel(this);
    treeProxy->setSourceModel(m_tree);
    treeProxy->setFilterRole(GGMediaTreeModel::TypeRole);
    treeProxy->setFilterKeyColumn(0);
    treeProxy->setFilterFixedString(GGMediaTreeModel::ENTRY_DIR);
    ui->treeFolders->setModel(treeProxy);

    ListProxyModel *listProxy = new ListProxyModel(this);
    listProxy->setSourceModel(m_tree);
    ui->lstMedia->setModel(listProxy);

    refresh();
}

GGMediaManagerDialog::~GGMediaManagerDialog()
{
    delete ui;
}

QString GGMediaManagerDialog::getSelectedMediaId()
{
    return m_acceptedSelection;
}

void GGMediaManagerDialog::refresh()
{
    m_acceptedSelection = QString::null;
    m_tree->reload();
    setSelectedDirectory(m_initSelection);
}

void GGMediaManagerDialog::setSelectedDirectory(const QString &dir)
{
    m_initSelection = dir;
    if (!m_initSelection.isEmpty()) {
        QSortFilterProxyModel *m = static_cast<QSortFilterProxyModel*> (ui->treeFolders->model());
        QModelIndexList lst = m->match(m->index(0,0), GGMediaTreeModel::PathRole, dir, 1, Qt::MatchRecursive | Qt::MatchEndsWith);
        if (lst.size() == 1) {
            ui->treeFolders->setCurrentIndex(lst[0]);
            treeItemSelected(lst[0]);
        }
    }
}

void GGMediaManagerDialog::expandAll()
{
    ui->treeFolders->expandAll();
}

void GGMediaManagerDialog::accept()
{
    QModelIndex idx = ui->lstMedia->currentIndex();
    m_acceptedSelection = ui->lstMedia->model()->data(idx, GGMediaTreeModel::IdRole).toString();
    QDialog::accept();
}

void GGMediaManagerDialog::treeItemSelected(QModelIndex idx)
{
    QSortFilterProxyModel *m = static_cast<QSortFilterProxyModel*> (ui->treeFolders->model());
    idx = m->mapToSource(idx);
    ListProxyModel *l = static_cast<ListProxyModel*> (ui->lstMedia->model());
    l->setRootIndex(idx);
    idx = l->mapFromSource(idx);
    ui->lstMedia->setRootIndex(idx);
}

void GGMediaManagerDialog::on_btnSynch_clicked()
{
    //m_initSelection = QString::null;
    refresh();
}
