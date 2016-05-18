#include "ggmainwindow.h"
#include "ui_ggmainwindow.h"
#include <QtWidgets>
#include <ui/dialogs/ggvariableeditdialog.h>
#include <model/ggeditproject.h>
#include <view/gguicontroller.h>
#include <view/ggeditorscene.h>
#include <viewmodel/ggviewscene.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <ui/dialogs/ggsearchdialog.h>
#include <ui/gggraphpanel.h>
#include <view/ggeditorview.h>
#include <view/ggpageitem.h>
#include <model/ggmediaresolver.h>
#include <view/ggscenetreemodel.h>
#include <ui/dialogs/ggcreateprojectdialog.h>
#include <ui/dialogs/ggmediamanagerdialog.h>
#include <model/ggscenemediamanager.h>
#include <ui/dialogs/ggrenamescenedlg.h>
#include <ui/dialogs/ggcreatescenedlg.h>
#include <io/ggviewprojectserializer.h>
#include <io/ggsimplexmlserializationwriter.h>
#include <io/ggserializationprocessor.h>
#include <io/ggviewprojectunserializer.h>
#include <ggutilities.h>
#include <utils/ggtrasher.h>
#include <ui/dialogs/ggstyledialog.h>
#include <style/ggabstractstyler.h>
#include <utils/ggglobaluserinfo.h>

GGMainWindow::GGMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GGMainWindow),
    m_project(NULL),
    m_viewModel(NULL),
    m_searchDlg(NULL),
    m_windowInfo(this, "MainWindow")
{
    ui->setupUi(this);

    m_baseTitle = windowTitle();
    updateWindowTitle();

    ui->dckSearchResults->setVisible(false);
    connect(ui->wgtSearchResults, SIGNAL(requestNewSearch()), this, SLOT(showSearchDialog()));
    connect(ui->wgtSearchResults, SIGNAL(highlightPage(GG::PageID)), this, SLOT(highlightPage(GG::PageID)));

    m_windowInfo.addSplitter(ui->splitter);
    m_windowInfo.addSplitter(ui->splitter_2);
    m_windowInfo.restore();

    m_ctrl = new GGUIController(this);
    ui->wgtPageContent->setController(m_ctrl);
    connect(m_ctrl, SIGNAL(commandError(QString)), this, SLOT(showError(QString)));
    connect(m_ctrl, SIGNAL(objectsSelected(QSet<GGViewPage*>,QSet<GGViewConnection*>)), this, SLOT(clearSelection()));
    connect(m_ctrl, SIGNAL(selectionCleared()), this, SLOT(clearSelection()));
    connect(m_ctrl, SIGNAL(singlePageSelected(GGViewPage*)), this, SLOT(selectPage(GGViewPage*)));
    connect(m_ctrl, SIGNAL(singleConnectionSelected(GGViewConnection*)), this, SLOT(selectConnection(GGViewConnection*)));
    connect(m_ctrl, SIGNAL(modelDirty(bool)), this, SLOT(setWindowModified(bool)));
    connect(m_ctrl, SIGNAL(modelDirty(bool)), ui->actionSave, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(hasProject(bool)), ui->actionSave_as_type, SLOT(setEnabled(bool)));
    connect(m_ctrl, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(m_ctrl, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
    connect(m_ctrl, SIGNAL(creationModeChanged(CreationMode)), this, SLOT(setCreationMode()));
    connect(m_ctrl, SIGNAL(connectingDirect(GGPage*,GGConnectionSlot)), this, SLOT(handleConnectDirect(GGPage*,GGConnectionSlot)));
    connect(ui->action_Variables, SIGNAL(triggered(bool)), this, SLOT(showVariables()));
    connect(ui->actionMedia, SIGNAL(triggered(bool)), this, SLOT(showMediaManager()));
    connect(ui->actionStyles, SIGNAL(triggered(bool)), this, SLOT(showStyleManager()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    connect(ui->actionSave_as_type, SIGNAL(triggered(bool)), this, SLOT(saveProjectAsType()));

    connect(ui->actionRenamePage, SIGNAL(triggered(bool)), ui->wgtPageContent, SLOT(editPageTitle()));
    connect(ui->actionEditCaption, SIGNAL(triggered(bool)), ui->wgtPageContent, SLOT(editPageCaption()));
    connect(ui->actionEditContent, SIGNAL(triggered(bool)), ui->wgtPageContent, SLOT(editPageContent()));
    connect(ui->actionEditMapping, SIGNAL(triggered(bool)), ui->wgtPageContent, SLOT(editPageContentMap()));

    connect(this, SIGNAL(hasProject(bool)), ui->wgtSearchResults, SLOT(clearResults()));

    this->addAction(ui->actionRenamePage);
    this->addAction(ui->actionEditCaption);
    this->addAction(ui->actionEditContent);
    this->addAction(ui->actionEditMapping);

    // Group Click Mode actions
    m_createActions = new QActionGroup(this);
    m_createActions->addAction(ui->actionS);
    m_createActions->addAction(ui->actionE);
    m_createActions->addAction(ui->actionC);
    m_createActions->addAction(ui->actionA);
    m_createActions->addAction(ui->actionD);
    m_createActions->addAction(ui->actionL);
    m_createActions->addAction(ui->actionP);

    ui->actionS->setIcon(GGPageItem::getPageTypeIcon(GGStartPage::Type));
    ui->actionE->setIcon(GGPageItem::getPageTypeIcon(GGEndPage::Type));
    ui->actionC->setIcon(GGPageItem::getPageTypeIcon(GGConditionPage::Type));
    ui->actionA->setIcon(GGPageItem::getPageTypeIcon(GGActionPage::Type));
    ui->actionD->setIcon(GGPageItem::getPageTypeIcon(GGDecisionPage::Type));

    // Set proper undo/redo shortcuts
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionFind->setShortcut(QKeySequence::Find);

    m_sceneTree = new GGSceneTreeModel(this);
    ui->treScenes->setModel(m_sceneTree);

    ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);

    // Store tab text of start page
    ui->tabStart->setWindowTitle(ui->tabScenes->tabText(ui->tabScenes->indexOf(ui->tabStart)));

    closeProject();
}

GGMainWindow::~GGMainWindow()
{
    closeProject();
    delete ui;
}

GGViewModel *GGMainWindow::currentModel()
{
    return m_viewModel;
}

GGUIController *GGMainWindow::controller()
{
    return m_ctrl;
}

GGGraphPanel *GGMainWindow::currentSceneView()
{
    GGGraphPanel *ret = qobject_cast<GGGraphPanel *> (ui->tabScenes->currentWidget());
    Q_ASSERT(ret || ui->tabScenes->currentWidget() == NULL || ui->tabScenes->currentWidget() == ui->tabStart);
    return ret;
}

GGGraphPanel *GGMainWindow::sceneViewForId(GG::SceneID id)
{
    return m_openScenes.value(id);
}

void GGMainWindow::connectModel()
{
    m_sceneTree->setModel(m_project->editModel());

    connect(m_viewModel, SIGNAL(viewSceneRegistered(GGViewScene*)), this, SLOT(openSceneView(GGViewScene*)));
    connect(m_viewModel, SIGNAL(viewSceneUnregistered(GGViewScene*)), this, SLOT(closeSceneView(GGViewScene*)));
    connect(m_viewModel, SIGNAL(sceneUpdated(GGViewScene*)), this, SLOT(updateTabs()));

    m_project->mediaManager()->synchronize();

    if (GGDefaultTrasher *t = dynamic_cast<GGDefaultTrasher*> (GGTrasher::trasher())) {
        t->setBaseDir(m_project->basePath().absoluteFilePath(GGDefaultTrasher::TRASH_DIR_NAME));
    }

    updateWindowTitle();
    emit projectOpened();
    emit hasProject(true);
}

void GGMainWindow::newProject()
{
    if (!closeProject()) {
        return;
    }

    GGCreateProjectDialog dlg;
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

#if defined(USE_VLD) && defined(VLD_PER_PROJECT)
    VLDGlobalEnable();
#endif

    m_project = new GGEditProject(dlg.projectBasePath(), GGUtilities::sanatizeFileName(dlg.projectTitle()), dlg.serializationType());
    m_project->setTitle(dlg.projectTitle());
    m_viewModel = new GGViewModel(m_project->editModel());

    m_ctrl->setProject(m_project, m_viewModel);
    m_ctrl->createDefaultScene(dlg.initialSceneName(), dlg.initialSceneDir());

    connectModel();

    saveProject();
}

bool GGMainWindow::closeProject()
{
    if (isWindowModified()) {
        int res = QMessageBox::question(this, "Save changes", "The current project has unsaved changes.\nDo you want to save these changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
        if (res == QMessageBox::Cancel) {
            return false;
        }
        if (res == QMessageBox::Yes) {
            saveProject();
        }
    }

    emit projectClosed();
    emit hasProject(false);

    // Reset Trasher
    if (GGDefaultTrasher *t = dynamic_cast<GGDefaultTrasher*> (GGTrasher::trasher())) {
        t->setBaseDir(QDir::temp().absoluteFilePath(GGDefaultTrasher::TRASH_DIR_NAME));
    }

    QList<GG::SceneID> openScenes = m_openScenes.keys();
    foreach (GG::SceneID id, openScenes) {
        GGViewScene *sc = m_openScenes[id]->editorScene()->modelScene();
        closeSceneView(sc);
    }

    m_openScenes.clear();
    ui->wgtPageContent->clearPage();
    m_sceneTree->setModel(NULL);
    m_ctrl->setProject(NULL, NULL);
    delete m_viewModel;
    m_viewModel = NULL;
    delete m_project;
    m_project = NULL;

    ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);

#if defined(USE_VLD) && defined(VLD_PER_PROJECT)
    VLDReportLeaks();
    VLDMarkAllLeaksAsReported();
    VLDGlobalDisable();
#endif

    showStartPage();
    updateWindowTitle();

    return true;
}

void GGMainWindow::saveProject()
{
    // Update scene positions
    foreach (GG::SceneID id, m_openScenes.keys()) {
        m_viewModel->getViewSceneForScene(m_project->model()->getScene(id))->setLoadPosition(m_openScenes[id]->editorView()->getScenePosition());
    }
    m_project->setOpenScenes(m_openScenes.keys());

    QFile f(m_project->basePath().absoluteFilePath(m_project->fileName()) + "." + GGIOFactory::extensionForSerializationType(m_project->saveType()));
    f.open(QIODevice::WriteOnly);
    GGViewProjectSerializer *ser = GGIOFactory::viewSerializer(&f, m_project->saveType());
    ser->saveProject(m_project, m_viewModel);
    delete ser;
    m_ctrl->saveCheckpoint();
}

void GGMainWindow::saveProjectAsType()
{
    bool ok;
    QStringList lst = QStringList() << "Single XML" << "Binary";
    int idx = m_project->saveType() == GGIOFactory::BinaryModel ? 1 : 0;

    QString itm = QInputDialog::getItem(this, "Save as type", "Select file type", lst, idx, false, &ok);
    if (!ok) return;
    idx = lst.indexOf(itm);
    if (idx <0) return;

    if (idx == 1) m_project->setSaveType(GGIOFactory::BinaryModel);
    else m_project->setSaveType(GGIOFactory::SimpleXMLModel);
    saveProject();
}

void GGMainWindow::openProject()
{
    if (!closeProject()) {
        return;
    }

    QString file = QFileDialog::getOpenFileName(this, "Open Project", QString(), "GameGenerator Projects (*.ggmb *.ggmx)");
    if (!file.isNull()) {
        QFile f(file);
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Cannot open file\n" + file);
            return;
        }

#if defined(USE_VLD) && defined(VLD_PER_PROJECT)
    VLDGlobalEnable();
#endif

        GGViewProjectUnserializer *ser = GGIOFactory::unserializer(file);
        bool ok = ser->load(&f);
        QString err = ser->error();
        GGEditProject *p = ser->takeProject();
        GGViewModel *v = ser->takeViewModel();
        delete ser;
        if (!ok || !p || !v) {
            QMessageBox::warning(this, "Error", "Error while loading: " + err);
            delete p;
            delete v;
            return;
        }

        m_project = p;
        m_viewModel = v;

        m_ctrl->setProject(m_project, m_viewModel);
        connectModel();

        foreach (GG::SceneID id, m_project->openScenes()) {
            GGViewScene *sc = m_viewModel->getViewSceneForScene(m_project->model()->getScene(id));
            if (sc) {
                openSceneView(sc);
            }
        }
    }
}

void GGMainWindow::openSceneView(GGViewScene *scene)
{
    GGGraphPanel *p = sceneViewForId(scene->scene()->id());
    if (!p) {
        p = new GGGraphPanel(m_ctrl, scene);

        // Always go back to pointer after a scene click
        connect(p->editorScene(), SIGNAL(clickedEmptySpace(GGViewScene*,QPointF)), this, SLOT(setPointerMode()));

        ui->tabScenes->addTab(p, scene->scene()->name());
        m_openScenes[scene->scene()->id()] = p;
    }
    ui->tabScenes->setCurrentWidget(p);
}

void GGMainWindow::closeSceneView(GGViewScene *scene)
{
    GGGraphPanel *p = sceneViewForId(scene->scene()->id());
    m_openScenes.remove(scene->scene()->id());
    if (p) {
        ui->tabScenes->removeTab(ui->tabScenes->indexOf(p));
        scene->setLoadPosition(p->editorView()->getScenePosition());
        delete p;
    }
}

void GGMainWindow::highlightPage(GG::PageID id)
{
    GGPage *p = m_viewModel->editModel()->getPage(id);
    GGViewPage *vp = m_viewModel->getViewPageForPage(p, p->sceneId());
    GGScene *sc = m_viewModel->editModel()->getScene(p->sceneId());
    GGViewScene *vs = m_viewModel->getViewSceneForScene(sc);
    openSceneView(vs);
    currentSceneView()->editorScene()->setSelection(QSet<GGViewPage*> () << vp, QSet<GGViewConnection*> ());
    currentSceneView()->editorView()->ensureVisible(vp->bounds());
    selectPage(vp);
}

void GGMainWindow::selectPage(GGViewPage *page)
{
    setPointerMode();
    if (!currentSceneView()) return;
    if (page->page()->sceneId() == currentSceneView()->editorScene()->modelScene()->scene()->id()) {
        ui->wgtPageContent->displayPage(page->page());
        ui->stkDetailEdits->setCurrentWidget(ui->pagePage);
    } else {
        ui->wgtPageContent->clearPage();
        ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);
    }
}

void GGMainWindow::selectConnection(GGViewConnection *conn)
{
    setPointerMode();
    ui->stkDetailEdits->setCurrentWidget(ui->pageLink);
    ui->wgtPageContent->clearPage();
}

void GGMainWindow::clearSelection()
{
    ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);
    ui->wgtPageContent->clearPage();
}

void GGMainWindow::showError(QString err)
{
    QMessageBox::critical(this, "Error", err);
}

void GGMainWindow::executeSearch(const GGSearchRequest &req)
{
    GGSearchResult lst = m_viewModel->editModel()->search(req);
    ui->wgtSearchResults->setResults(lst);
    ui->dckSearchResults->setVisible(true);
}

void GGMainWindow::showSearchDialog()
{
    showSearchDialog(true);
}

void GGMainWindow::showSearchDialog(bool reset)
{
    if (!m_searchDlg) {
        m_searchDlg = new GGSearchDialog(this);
        connect(m_searchDlg, SIGNAL(executeSearch(GGSearchRequest)), this, SLOT(executeSearch(GGSearchRequest)));
    }
    if (reset) {
        m_searchDlg->resetSearch();
    }
    if (currentSceneView())
        m_searchDlg->setCurrentSceneId(currentSceneView()->editorScene()->modelScene()->scene()->id());
    else
        m_searchDlg->setCurrentSceneId(GG::InvalidSceneId);
    m_searchDlg->show();
    m_searchDlg->activateWindow();
}

void GGMainWindow::setClickMode(QAction *act)
{
    if (!currentSceneView()) return;
    if (act == ui->actionP) {
        currentSceneView()->editorView()->setDragMode(QGraphicsView::RubberBandDrag);
    } else {
        currentSceneView()->editorView()->setDragMode(QGraphicsView::NoDrag);
        currentSceneView()->editorScene()->clearSelection();
    }

    bool ok;
    QMetaEnum cmEnum = GGUIController::staticMetaObject.enumerator(GGUIController::staticMetaObject.indexOfEnumerator("CreationMode"));
    int cm = cmEnum.keyToValue(act->property("CreationMode").toString().toUtf8(), &ok);
    Q_ASSERT_X(ok, "GGMainWindow::setClickMode", "Invalid Creation Mode");
    if (ok) {
        m_ctrl->setCreationMode(static_cast<GGUIController::CreationMode> (cm));
    }
}

void GGMainWindow::setPointerMode()
{
    ui->actionP->activate(QAction::Trigger);
}

void GGMainWindow::handleAction(QAction *act)
{
    if (m_createActions->actions().contains(act)) {
        setClickMode(act);
        return;
    }
    if (act == ui->actionUndo) {
        m_ctrl->undo();
        return;
    }
    if (act == ui->actionRedo) {
        m_ctrl->redo();
        return;
    }
    if (act == ui->actionDelete) {
        if (currentSceneView())
            currentSceneView()->editorScene()->deleteCurrentSelection();
    }
    if (act == ui->actionFind) {
        showSearchDialog(false);
    }
}

void GGMainWindow::setCreationMode()
{
    QAction *toActivate = NULL;
    foreach (QAction *act, m_createActions->actions()) {
        act->setChecked(false);
        bool ok;
        QMetaEnum cmEnum = GGUIController::staticMetaObject.enumerator(GGUIController::staticMetaObject.indexOfEnumerator("CreationMode"));
        int cm = cmEnum.keyToValue(act->property("CreationMode").toString().toUtf8(), &ok);
        Q_ASSERT_X(ok, "GGMainWindow::setCreationMode", "Invalid Creation Mode");
        if (cm == m_ctrl->creationMode()) {
            toActivate = act;
            break;
        }
    }

    if (toActivate)
        toActivate->activate(QAction::Trigger);
}

void GGMainWindow::handleConnectDirect(GGPage *page, const GGConnectionSlot &slot)
{
    ui->stkDetailEdits->setCurrentWidget(ui->pageLinkDirect);
    ui->wgtPageContent->clearPage();
}

void GGMainWindow::showVariables()
{
    GGVariableEditDialog dlg;
    dlg.setModel(m_ctrl->model()->editModel());
    connect(&dlg, SIGNAL(showUsages(GGSearchRequest)), this, SLOT(executeSearch(GGSearchRequest)));
    dlg.exec();
    m_ctrl->applySubcommandsAsGroup(dlg.getExecutedCommands());

    // Quick hack so that Actions/Conditions will have up to date variables:
    // Reselect after closing
    if (ui->wgtPageContent->displayedPage())
        ui->wgtPageContent->displayPage(ui->wgtPageContent->displayedPage());
}

void GGMainWindow::showMediaManager()
{
    GGMediaManagerDialog dlg(m_project->mediaManager());
    dlg.expandAll();
    dlg.exec();
}

void GGMainWindow::showStyleManager()
{
    GGStyleDialog dlg(this);
    dlg.setModel(m_viewModel->editModel());
    if (dlg.exec() == QDialog::Accepted) {
        m_ctrl->applySubcommandsAsGroup(dlg.getExecutedCommands());
    } else {
        dlg.getExecutedCommands()->undo();
    }
}

void GGMainWindow::showStartPage()
{
    if (ui->tabScenes->indexOf(ui->tabStart) < 0) {
        // Re-Open start page if all tab are closed
        ui->tabScenes->addTab(ui->tabStart, ui->tabStart->windowTitle());
    }
    ui->tabScenes->setCurrentWidget(ui->tabStart);
}

void GGMainWindow::closeEvent(QCloseEvent *event)
{
    if (closeProject()) {
        m_windowInfo.backup();
        event->accept();
    } else {
        event->ignore();
    }
}

void GGMainWindow::closeTab(int idx)
{
    if (idx == ui->tabScenes->indexOf(ui->tabStart)) {
        if (!m_project) {
            // Don't close start page if there is no project open
            return;
        }
    }

    GGGraphPanel *p = qobject_cast<GGGraphPanel *> (ui->tabScenes->widget(idx));
    if (p) {
        closeSceneView(p->editorScene()->modelScene());
    } else {
        ui->tabScenes->removeTab(idx);
        delete p;
    }

    if (ui->tabScenes->count() == 0) {
        showStartPage();
    }
}

void GGMainWindow::changeTab(int idx)
{
    if (idx < 0) return;

    GGGraphPanel *p = qobject_cast<GGGraphPanel *> (ui->tabScenes->widget(idx));
    if (p)
        p->editorScene()->clearSelection();

    if (m_ctrl->creationMode() != GGUIController::CreateConnectionDirect)
        setPointerMode();
}

void GGMainWindow::updateTabs()
{
    for (int i = 0; i < ui->tabScenes->count(); ++i) {
        GGGraphPanel *p = qobject_cast<GGGraphPanel *> (ui->tabScenes->widget(i));
        if (p) {
            ui->tabScenes->setTabText(i, p->editorScene()->modelScene()->scene()->name());
        }
    }
}

void GGMainWindow::sceneTreeActivated(const QModelIndex &idx)
{
    GG::SceneID id = static_cast<GG::SceneID> (m_sceneTree->data(idx, GGSceneTreeModel::SceneIdRole).toInt());
    GGScene *sc = m_viewModel->editModel()->getScene(id);
    GGViewScene *vs = m_viewModel->getViewSceneForScene(sc);
    openSceneView(vs);
}

void GGMainWindow::showSceneTreeContextMenu(const QPoint &point)
{
    if (!m_project) return;
    QMenu mnu;
    QModelIndex idx = ui->treScenes->indexAt(point);
    if (idx.isValid()) {
        GG::SceneID id = static_cast<GG::SceneID> (m_sceneTree->data(idx, GGSceneTreeModel::SceneIdRole).toInt());
        GGScene *s = m_viewModel->editModel()->getScene(id);
        if (s) {
            QAction *ren = new QAction("Rename...", &mnu);
            ren->setData(id);
            QAction *del = new QAction("Delete", &mnu);
            del->setData(id);

            connect(ren, SIGNAL(triggered(bool)), this, SLOT(renameSceneAction()));
            connect(del, SIGNAL(triggered(bool)), this, SLOT(deleteSceneAction()));

            mnu.addAction(ren);
            mnu.addAction(del);
            mnu.addSeparator();
        }
    }

    QAction *cre = new QAction("New scene...", &mnu);
    connect(cre, SIGNAL(triggered(bool)), this, SLOT(createSceneAction()));
    mnu.addAction(cre);
    mnu.exec(ui->treScenes->mapToGlobal(point));
}

void GGMainWindow::renameSceneAction()
{
    GG::SceneID id = static_cast<GG::SceneID> (static_cast<QAction*> (sender())->data().toInt());
    GGScene *s = m_viewModel->editModel()->getScene(id);
    if (s) {
        GGRenameSceneDlg dlg(s, this);
        if (dlg.exec() == QDialog::Accepted) {
            QString newMedia = dlg.renameDir() ? dlg.newMedia() : QString::null;
            m_ctrl->renameScene(s, dlg.newName(), newMedia);
        }
    }
}

void GGMainWindow::deleteSceneAction()
{
    GG::SceneID id = static_cast<GG::SceneID> (static_cast<QAction*> (sender())->data().toInt());
    GGScene *s = m_viewModel->editModel()->getScene(id);
    if (s) {
        m_ctrl->deleteScene(s);
    }
}

void GGMainWindow::createSceneAction()
{
    GGCreateSceneDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        m_ctrl->createScene(dlg.sceneName(), dlg.sceneDir());
    }
}

void GGMainWindow::on_action_About_triggered()
{
    QMessageBox::information(this, "GameGenerator", QString("GameGenerator V%1\n"
                                                            "Copyright © by King Nak\n"
                                                            "This is a incomplete preview version!")
                             .arg(GGUtilities::versionToString(GG::VERSION)));
}

void GGMainWindow::updateWindowTitle()
{
    if (m_project) {
        setWindowTitle(m_baseTitle.arg(m_project->title(), " - "));
    } else {
        setWindowTitle(m_baseTitle.arg("", ""));
    }
}
