#include "ggmainwindow.h"
#include "ui_ggmainwindow.h"
#include <QtWidgets>
#include <view/gguicontroller.h>
#include <view/ggeditorscene.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggpage.h>

GGMainWindow::GGMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GGMainWindow),
    m_viewModel(NULL)
{
    ui->setupUi(this);

    m_ctrl = new GGUIController(this);
    connect(m_ctrl, SIGNAL(commandError(QString)), this, SLOT(showError(QString)));
    connect(m_ctrl, SIGNAL(objectsSelected(QSet<GGViewPage*>,QSet<GGViewConnection*>)), this, SLOT(clearSelection()));
    connect(m_ctrl, SIGNAL(selectionCleared()), this, SLOT(clearSelection()));
    connect(m_ctrl, SIGNAL(singlePageSelected(GGViewPage*)), this, SLOT(selectPage(GGViewPage*)));
    connect(m_ctrl, SIGNAL(singleConnectionSelected(GGViewConnection*)), this, SLOT(selectConnection(GGViewConnection*)));
    connect(m_ctrl, SIGNAL(modelDirty(bool)), this, SLOT(setWindowModified(bool)));
    connect(m_ctrl, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(m_ctrl, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

    m_editorScene = new GGEditorScene(m_ctrl, this);
    m_editorScene->setSceneRect(-400,-400,800,800);
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

    // Always go back to pointer after a scene click
    connect(m_editorScene, SIGNAL(clickedEmptySpace(QPointF)), this, SLOT(setPointerMode()));

    newModel();
}

GGMainWindow::~GGMainWindow()
{
    closeModel();
    delete ui;
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
    switch (page->page()->type()) {
    case GGStartPage::Type:
        ui->stkDetailEdits->setCurrentWidget(ui->pageStart);
        break;
    case GGEndPage::Type:
        ui->stkDetailEdits->setCurrentWidget(ui->pageEnd);
        break;
    case GGConditionPage::Type:
        ui->stkDetailEdits->setCurrentWidget(ui->pageCondition);
        break;
    case GGActionPage::Type:
        ui->stkDetailEdits->setCurrentWidget(ui->pageAction);
        break;
    case GGDecisionPage::Type:
        ui->stkDetailEdits->setCurrentWidget(ui->pageDecision);
        break;
    default:
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

void GGMainWindow::setClickMode(QAction *act)
{
    if (act == ui->actionP) {
        ui->scEditView->setDragMode(QGraphicsView::RubberBandDrag);
    } else {
        ui->scEditView->setDragMode(QGraphicsView::NoDrag);
        m_editorScene->clearSelection();
    }

    bool ok;
    int cm = QMetaEnum::fromType<GGUIController::CreationMode>().keyToValue(act->property("CreationMode").toString().toUtf8(), &ok);
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
}
