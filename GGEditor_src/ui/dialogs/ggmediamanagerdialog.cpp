#include "ggmediamanagerdialog.h"
#include "ui_ggmediamanagerdialog.h"
#include <QSortFilterProxyModel>
#include <QtWidgets>
#include <utils/ggtrasher.h>
#include <model/ggscenemediamanager.h>
#include <view/ggmediatreemodel.h>
#include <model/ggeditmodel.h>

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

GGMediaManagerDialog::GGMediaManagerDialog(GGSceneMediaManager *mgm, QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    ui(new Ui::GGMediaManagerDialog),
    m_requestingMedia(false),
    m_windowInfo(this, "MediaManager")
{
    ui->setupUi(this);
    // Don't save splitter, always make as small as possible
    m_windowInfo.restore();
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes(QList<int>() << ui->treeFolders->minimumSize().width());
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
    ui->btnRemove->setEnabled(false);

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
    static_cast<ListProxyModel*> (ui->lstMedia->model())->setRootIndex(QModelIndex());
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
    m_windowInfo.backup();
    QDialog::accept();
}

void GGMediaManagerDialog::reject()
{
    m_windowInfo.backup();
    QDialog::reject();
}

int GGMediaManagerDialog::requestMedia()
{
    m_requestingMedia = true;
    return exec();
}

void GGMediaManagerDialog::closeEvent(QCloseEvent *event)
{
    m_windowInfo.backup();
    event->accept();
}

void GGMediaManagerDialog::treeItemSelected(QModelIndex idx)
{
    QSortFilterProxyModel *m = static_cast<QSortFilterProxyModel*> (ui->treeFolders->model());
    ui->treeFolders->setCurrentIndex(idx);
    idx = m->mapToSource(idx);
    ListProxyModel *l = static_cast<ListProxyModel*> (ui->lstMedia->model());
    l->setRootIndex(idx);
    idx = l->mapFromSource(idx);
    ui->lstMedia->setRootIndex(idx);
    ui->btnRemove->setEnabled(false);
}

void GGMediaManagerDialog::listItemSelected()
{
    if (ui->lstMedia->currentIndex().isValid()) {
        ui->btnRemove->setEnabled(true);
    } else {
        ui->btnRemove->setEnabled(false);
    }
}

void GGMediaManagerDialog::on_btnSynch_clicked()
{
    //m_initSelection = QString::null;
    refresh();
}

void GGMediaManagerDialog::on_btnVerify_clicked()
{
    QStringList lst = m_tree->verify();
    if (lst.isEmpty()) return;

    QInputDialog dlg(this);
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
    dlg.setWindowTitle("Missing media");
    dlg.setLabelText("The following files were not found on the file system:");
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
    dlg.setComboBoxItems(lst);
    dlg.setTextValue(lst[0]);
    dlg.exec();
}

void GGMediaManagerDialog::on_btnCleanup_clicked()
{
    static_cast<ListProxyModel*> (ui->lstMedia->model())->setRootIndex(QModelIndex());
    QStringList lst = m_tree->cleanUp();
    if (lst.isEmpty()) return;

    QInputDialog dlg(this);
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
    dlg.setWindowTitle("Removed media");
    dlg.setLabelText("The following files were removed from the manager:");
    dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
    dlg.setComboBoxItems(lst);
    dlg.setTextValue(lst[0]);
    dlg.exec();
}

void GGMediaManagerDialog::on_btnAdd_clicked()
{
    QStringList imgs = GGMediaManager::getMediaSuffixes(GGMediaManager::Image);
    imgs.replaceInStrings(QRegExp("^(.*)$"), "*.\\1");
    QStringList vids = GGMediaManager::getMediaSuffixes(GGMediaManager::Video);
    vids.replaceInStrings(QRegExp("^(.*)$"), "*.\\1");
    QStringList auds = GGMediaManager::getMediaSuffixes(GGMediaManager::Audio);
    auds.replaceInStrings(QRegExp("^(.*)$"), "*.\\1");

    QStringList all = imgs + vids + auds;

    bool ok;
    GG::SceneID sid = static_cast<GG::SceneID> (m_tree->data(selectedTreeIndex(), GGMediaTreeModel::SceneRole).toInt(&ok));
    if (!ok) sid = GG::InvalidSceneId;

    GGMediaManager::MediaType mt = static_cast<GGMediaManager::MediaType> (m_tree->data(selectedTreeIndex(), GGMediaTreeModel::MediaTypeRole).toInt(&ok));
    if (!ok) mt = GGMediaManager::Other;

    QStringList filters = QStringList()
            << QString("Images (%1)").arg(imgs.join(" "))
            << QString("Videos (%1)").arg(vids.join(" "))
            << QString("Sounds (%1)").arg(auds.join(" "))
            << QString("All supported media files (%1)").arg(all.join(" "))
            << QString("All files (*)")
               ;

    QString selected = filters[3];
    if (mt == GGMediaManager::Image) selected = filters[0];
    if (mt == GGMediaManager::Video) selected = filters[1];
    if (mt == GGMediaManager::Audio) selected = filters[2];

    QString openDir = m_tree->manager()->getMediaSourceDir();
    if (openDir.isEmpty()) {
        openDir = QDir::homePath();
    }
    QStringList files = QFileDialog::getOpenFileNames(this, "Open File", openDir, filters.join(";;"), &selected);
    QStringList err;
    foreach (QString f, files) {
        QString id = m_tree->manager()->checkIn(m_tree->manager()->model()->getScene(sid), f);
        if (id.isNull()) err << f;

        // Update open dir path
        QFileInfo fi(f);
        m_tree->manager()->setMediaSourceDir(fi.absolutePath());
    }

    if (err.size() > 0) {
        QInputDialog dlg(this);
        dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
        dlg.setWindowTitle("Error");
        dlg.setLabelText("Could not add the following files:");
        dlg.setOption(QInputDialog::UseListViewForComboBoxItems);
        dlg.setComboBoxItems(err);
        dlg.setTextValue(err[0]);
        dlg.exec();
    }

    QString dir = m_tree->data(selectedTreeIndex(), GGMediaTreeModel::PathRole).toString();
    refresh();
    setSelectedDirectory(dir);
}

void GGMediaManagerDialog::on_btnRemove_clicked()
{
    QModelIndex idx = selectedListIndex();
    if (!idx.isValid()) return;

    QString path = m_tree->data(idx, GGMediaTreeModel::PathRole).toString();

    QFileInfo fi(path);
    QString fn = fi.fileName();

    QMessageBox msg(this);
    msg.setWindowTitle("Remove file");
    msg.setText(QString("Do you want to delete the file\n%1\npermanently or move to trash?").arg(fn));
    QPushButton *del = msg.addButton("Delete permanently", QMessageBox::DestructiveRole);
    QPushButton *trash = msg.addButton("Move to trash", QMessageBox::AcceptRole);
    QPushButton *cancel = msg.addButton("Cancel", QMessageBox::RejectRole);
    msg.setDefaultButton(trash);
    msg.setEscapeButton(cancel);
    msg.exec();

    // Cancel
    if (msg.clickedButton() == cancel) return;

    m_tree->manager()->removeManagedFile(path);

    if (msg.clickedButton() == del) {
        // DELETE
        QFile::remove(path);
    } else {
        // Move to trash
        GGTrasher::trasher()->moveToTrash(path);
    }

    QString dir = m_tree->data(selectedTreeIndex(), GGMediaTreeModel::PathRole).toString();
    refresh();
    setSelectedDirectory(dir);
}

void GGMediaManagerDialog::acceptMedia(QModelIndex idx)
{
    if (!m_requestingMedia) return;
    ui->lstMedia->setCurrentIndex(idx);
    accept();
}

QModelIndex GGMediaManagerDialog::selectedTreeIndex()
{
    QModelIndex idx = ui->treeFolders->currentIndex();
    QSortFilterProxyModel *m = static_cast<QSortFilterProxyModel*> (ui->treeFolders->model());
    return m->mapToSource(idx);
}

QModelIndex GGMediaManagerDialog::selectedListIndex()
{
    QModelIndex idx = ui->lstMedia->currentIndex();
    QSortFilterProxyModel *m = static_cast<QSortFilterProxyModel*> (ui->lstMedia->model());
    return m->mapToSource(idx);
}
