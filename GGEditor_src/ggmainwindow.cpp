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

    m_editorScene = new GGEditorScene(m_ctrl, this);
    m_editorScene->setSceneRect(-400,-400,800,800);
    ui->scEditView->setScene(m_editorScene);

    connect(m_editorScene, SIGNAL(pageSelected(GGViewPage*)), this, SLOT(selectPage(GGViewPage*)));
    connect(m_editorScene, SIGNAL(connectionSelected(GGViewConnection*)), this, SLOT(selectConnection(GGViewConnection*)));
    connect(m_editorScene, SIGNAL(otherSelected()), this, SLOT(selectOther()));
    connect(m_editorScene, SIGNAL(clickedEmptySpace(QPointF)), this, SLOT(clickedEmptySpace(QPointF)));


    m_createActions = new QActionGroup(this);
    m_createActions->addAction(ui->actionS);
    m_createActions->addAction(ui->actionE);
    m_createActions->addAction(ui->actionC);
    m_createActions->addAction(ui->actionA);
    m_createActions->addAction(ui->actionD);
    m_createActions->addAction(ui->actionL);
    m_createActions->addAction(ui->actionP);

    connect(m_createActions, SIGNAL(triggered(QAction*)), this, SLOT(setClickMode(QAction*)));

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
    m_editorScene->selectPage(page);
}

void GGMainWindow::selectConnection(GGViewConnection *conn)
{
    setPointerMode();
    ui->stkDetailEdits->setCurrentWidget(ui->pageLink);
}

void GGMainWindow::selectOther()
{
    //setPointerMode();
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
}

void GGMainWindow::clickedEmptySpace(QPointF pos)
{
    QAction *act = m_createActions->checkedAction();
    QRect r(pos.toPoint(), QSize());
    if (act == ui->actionS) {
        selectPage(m_ctrl->createStartPage(r));
    } else if (act == ui->actionE) {
        selectPage(m_ctrl->createEndPage(r));
    } else if (act == ui->actionC) {
        selectPage(m_ctrl->createConditionPage(r));
    } else if (act == ui->actionA) {
        selectPage(m_ctrl->createActionPage(r));
    } else if (act == ui->actionD) {
        selectPage(m_ctrl->createDecisionPage(r));
    } else if (act == ui->actionL) {
qDebug("L");
    } else if (act == ui->actionP) {
qDebug("P");
    } else {
qDebug("?");
    }
    setPointerMode();
}

void GGMainWindow::setPointerMode()
{
    ui->actionP->activate(QAction::Trigger);

}
