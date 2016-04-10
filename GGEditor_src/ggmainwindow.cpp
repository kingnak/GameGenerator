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

GGMainWindow::GGMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GGMainWindow),
    m_project(NULL),
    m_viewModel(NULL),
    m_searchDlg(NULL)
{
    ui->setupUi(this);

    m_baseTitle = windowTitle();
    updateWindowTitle();

    ui->dckSearchResults->setVisible(false);
    connect(ui->wgtSearchResults, SIGNAL(requestNewSearch()), this, SLOT(showSearchDialog()));
    connect(ui->wgtSearchResults, SIGNAL(highlightPage(GG::PageID)), this, SLOT(highlightPage(GG::PageID)));

    m_ctrl = new GGUIController(this);
    ui->wgtPageContent->setController(m_ctrl);
    connect(m_ctrl, SIGNAL(commandError(QString)), this, SLOT(showError(QString)));
    connect(m_ctrl, SIGNAL(objectsSelected(QSet<GGViewPage*>,QSet<GGViewConnection*>)), this, SLOT(clearSelection()));
    connect(m_ctrl, SIGNAL(selectionCleared()), this, SLOT(clearSelection()));
    connect(m_ctrl, SIGNAL(singlePageSelected(GGViewPage*)), this, SLOT(selectPage(GGViewPage*)));
    connect(m_ctrl, SIGNAL(singleConnectionSelected(GGViewConnection*)), this, SLOT(selectConnection(GGViewConnection*)));
    connect(m_ctrl, SIGNAL(modelDirty(bool)), this, SLOT(setWindowModified(bool)));
    connect(m_ctrl, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(m_ctrl, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
    connect(m_ctrl, SIGNAL(creationModeChanged(CreationMode)), this, SLOT(setCreationMode()));
    connect(m_ctrl, SIGNAL(connectingDirect(GGPage*,GGConnectionSlot)), this, SLOT(handleConnectDirect(GGPage*,GGConnectionSlot)));
    connect(ui->action_Variables, SIGNAL(triggered(bool)), this, SLOT(showVariables()));

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

void GGMainWindow::newProject()
{
    closeProject();

    GGCreateProjectDialog dlg;
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    m_project = new GGEditProject;
    m_project->setTitle(dlg.projectTitle());
    m_project->setBasePath(dlg.projectBasePath());
    m_viewModel = new GGViewModel(m_project->editModel());
    m_ctrl->setModel(m_viewModel);
    m_sceneTree->setModel(m_project->editModel());

    connect(m_viewModel, SIGNAL(viewSceneRegistered(GGViewScene*)), this, SLOT(openSceneView(GGViewScene*)));
    connect(m_viewModel, SIGNAL(viewSceneUnregistered(GGViewScene*)), this, SLOT(closeSceneView(GGViewScene*)));
    connect(m_viewModel, SIGNAL(sceneUpdated(GGViewScene*)), this, SLOT(updateTabs()));

    m_ctrl->createDefaultScene(dlg.initialSceneName());

    updateWindowTitle();
}

void GGMainWindow::closeProject()
{
    while (ui->tabScenes->count() > 0)
        ui->tabScenes->removeTab(0);

    m_openScenes.clear();
    m_sceneTree->setModel(NULL);
    m_ctrl->setModel(NULL);
    delete m_viewModel;
    m_viewModel = NULL;
    delete m_project;
    m_project = NULL;

    ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);

    showStartPage();
    updateWindowTitle();
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
        ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);
    }
}

void GGMainWindow::selectConnection(GGViewConnection *conn)
{
    setPointerMode();
    ui->stkDetailEdits->setCurrentWidget(ui->pageLink);
}

void GGMainWindow::clearSelection()
{
    ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);
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
}

void GGMainWindow::showVariables()
{
    GGVariableEditDialog dlg;
    dlg.setModel(m_ctrl->model()->editModel());
    connect(&dlg, SIGNAL(showUsages(GGSearchRequest)), this, SLOT(executeSearch(GGSearchRequest)));
    dlg.exec();
    m_ctrl->applySubcommandsAsGroup(dlg.getExecutedCommands());
}

void GGMainWindow::showStartPage()
{
    if (ui->tabScenes->indexOf(ui->tabStart) < 0) {
        // Re-Open start page if all tab are closed
        ui->tabScenes->addTab(ui->tabStart, ui->tabStart->windowTitle());
    }
    ui->tabScenes->setCurrentWidget(ui->tabStart);
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
    if (p)
        m_openScenes.remove(p->editorScene()->modelScene()->scene()->id());

    ui->tabScenes->removeTab(idx);
    delete p;

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
        QString oldName = s->name();
        QString newName = QInputDialog::getText(this, "Rename Scene", "Enter new name:", QLineEdit::Normal, oldName);
        if (!newName.isEmpty()) {
            m_ctrl->renameScene(s, newName);
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
    QString name = QInputDialog::getText(this, "Create Scene", "Enter scene name:");
    if (!name.isEmpty()) {
        m_ctrl->createScene(name);
    }
}

void GGMainWindow::updateWindowTitle()
{
    if (m_project) {
        setWindowTitle(m_baseTitle.arg(m_project->title(), " - "));
    } else {
        setWindowTitle(m_baseTitle.arg("", ""));
    }
}
