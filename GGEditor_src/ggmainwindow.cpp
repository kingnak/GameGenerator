#include "ggmainwindow.h"
#include "ui_ggmainwindow.h"
#include <QtWidgets>
#include <ui/dialogs/ggvariableeditdialog.h>
#include <view/gguicontroller.h>
#include <view/ggeditorscene.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggpage.h>
#include <ui/dialogs/ggsearchdialog.h>

GGMainWindow::GGMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GGMainWindow),
    m_viewModel(NULL),
    m_searchDlg(NULL)
{
    ui->setupUi(this);

    ui->dckSearchResults->setVisible(false);
    connect(ui->wgtSearchResults, SIGNAL(requestNewSearch()), this, SLOT(showSearchDialog()));

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

    m_editorScene = new GGEditorScene(m_ctrl, this);
    //m_editorScene->setSceneRect(0,0,800,800);
    ui->scEditView->setScene(m_editorScene);

    // Group Click Mode actions
    m_createActions = new QActionGroup(this);
    m_createActions->addAction(ui->actionS);
    m_createActions->addAction(ui->actionE);
    m_createActions->addAction(ui->actionC);
    m_createActions->addAction(ui->actionA);
    m_createActions->addAction(ui->actionD);
    m_createActions->addAction(ui->actionL);
    m_createActions->addAction(ui->actionP);

    // Set proper undo/redo shortcuts
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionFind->setShortcut(QKeySequence::Find);

    // Always go back to pointer after a scene click
    connect(m_editorScene, SIGNAL(clickedEmptySpace(QPointF)), this, SLOT(setPointerMode()));

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

void GGMainWindow::newModel()
{
    closeModel();
    GGEditModel *em = new GGEditModel(new GGSimpleFactory);
    m_viewModel = new GGViewModel(em);
    m_ctrl->setModel(m_viewModel);
    ui->scEditView->setEnabled(true);
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
    ui->scEditView->setEnabled(false);
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
        ui->scEditView->setDragMode(QGraphicsView::RubberBandDrag);
    } else {
        ui->scEditView->setDragMode(QGraphicsView::NoDrag);
        m_editorScene->clearSelection();
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
        m_editorScene->deleteCurrentSelection();
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
