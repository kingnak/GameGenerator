#include "ggmainwindow.h"
#include "ui_ggmainwindow.h"
#include <QtWidgets>
#include <ui/dialogs/ggvariableeditdialog.h>
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

GGMainWindow::GGMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GGMainWindow),
    m_viewModel(NULL),
    m_searchDlg(NULL)
{
    ui->setupUi(this);

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

    newModel();
}

GGMainWindow::~GGMainWindow()
{
    closeModel();
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
    Q_ASSERT(ret || ui->tabScenes->currentWidget() == NULL);
    return ret;
}

GGGraphPanel *GGMainWindow::sceneViewForId(GG::SceneID id)
{
    for (int i = 0; i < ui->tabScenes->count(); ++i) {
        GGGraphPanel *p = qobject_cast<GGGraphPanel *> (ui->tabScenes->widget(i));
        Q_ASSERT(p);
        if (p->editorScene()->modelScene()->scene()->id() == id) {
            return p;
        }
    }
    return NULL;
}

void GGMainWindow::newModel()
{
    closeModel();
    GGEditModel *em = new GGEditModel(new GGSimpleFactory, new GGFileSystemResolver);

    m_viewModel = new GGViewModel(em);
    m_ctrl->setModel(m_viewModel);
    m_sceneTree->setModel(em);

    connect(em, SIGNAL(sceneRegistered(GGScene*)), this, SLOT(openSceneView(GGScene*)));
    connect(em, SIGNAL(sceneUnregistered(GG::SceneID,GGScene*)), this, SLOT(closeSceneView(GG::SceneID)));

    // TODO: As Command???? => No, must always be there, cannot be undone
    GGScene *defaultScene = new GGScene;
    defaultScene->setName("Default");
    em->registerNewScene(defaultScene);
}

void GGMainWindow::closeModel()
{
    m_ctrl->setModel(NULL);
    if (m_viewModel) {
        delete m_viewModel->editModel();
        delete m_viewModel;
    }
    m_viewModel = NULL;
    ui->stkDetailEdits->setCurrentWidget(ui->pageEmpty);

    while (ui->tabScenes->count() > 0)
        ui->tabScenes->removeTab(0);
}

void GGMainWindow::openSceneView(GGScene *scene)
{
    GGGraphPanel *p = sceneViewForId(scene->id());
    if (!p) {
        GGViewScene *vsc = m_viewModel->getViewSceneForScene(scene);
        Q_ASSERT(vsc);
        p = new GGGraphPanel(m_ctrl, vsc);

        // Always go back to pointer after a scene click
        connect(p->editorScene(), SIGNAL(clickedEmptySpace(GGViewScene*,QPointF)), this, SLOT(setPointerMode()));

        ui->tabScenes->addTab(p, scene->name());
    }
}

void GGMainWindow::closeSceneView(GG::SceneID id)
{
    GGGraphPanel *p = sceneViewForId(id);
    if (p) {
        ui->tabScenes->removeTab(ui->tabScenes->indexOf(p));
    }
}

void GGMainWindow::highlightPage(GG::PageID id)
{
    GGPage *p = m_viewModel->editModel()->getPage(id);
    GGViewPage *vp = m_viewModel->getViewPageForPage(p, p->sceneId());
    GGScene *sc = m_viewModel->editModel()->getScene(p->sceneId());
    openSceneView(sc);
    currentSceneView()->editorScene()->setSelection(QSet<GGViewPage*> () << vp, QSet<GGViewConnection*> ());
    currentSceneView()->editorView()->ensureVisible(vp->bounds());
    selectPage(vp);
}

void GGMainWindow::selectPage(GGViewPage *page)
{
    setPointerMode();
    ui->wgtPageContent->displayPage(page->page());
    ui->stkDetailEdits->setCurrentWidget(ui->pagePage);
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
    dlg.exec();
}

void GGMainWindow::closeTab(int idx)
{
    ui->tabScenes->removeTab(idx);
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
