#include "ggmainwindow.h"
/*
#include <model/ggsimplefactory.h>
#include <model/ggeditmodel.h>
*/
#include <viewmodel/ggviewmodel.h>
#include <viewcommand/ggviewcommandfactory.h>
#include <viewcommand/ggviewcommands.h>
#include <view/ggeditorscene.h>
#include <view/gguicontroller.h>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QAction>

GGMainWindow::GGMainWindow()
{
    /*
    GGEditModel *em = new GGEditModel(new GGSimpleFactory, this);
    GGViewModel *vm = new GGViewModel(em, this);
    GGEditorScene *sc = new GGEditorScene(vm, this);
    sc->setSceneRect(-400,-400,800,800);
    QGraphicsView *v = new QGraphicsView(sc);
    setCentralWidget(v);
    GGViewCommandFactory *fac = new GGViewCommandFactory(vm);

    QGraphicsEllipseItem *e = new QGraphicsEllipseItem(-5,-5,10,10);
    e->setBrush(Qt::red);
    e->setZValue(100000);
    sc->addItem(e);

    GGAbstractCommandFactory::oneShotCommand(fac->createStartPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createEndPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createConditionPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createActionPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createDecisionPage(QRect(0,0,100,100)));
    delete fac;
    */
    m_ctrl = new GGUIController(this);

    GGEditorScene *sc = new GGEditorScene(m_ctrl->model(), this);
    sc->setSceneRect(-400,-400,800,800);
    QGraphicsView *v = new QGraphicsView(sc);
    setCentralWidget(v);
    GGViewCommandFactory *fac = new GGViewCommandFactory(m_ctrl->model());

    QGraphicsEllipseItem *e = new QGraphicsEllipseItem(-5,-5,10,10);
    e->setBrush(Qt::red);
    e->setZValue(100000);
    sc->addItem(e);

    connect(sc, SIGNAL(pageMoved(GGViewPage*,QRect)), m_ctrl, SLOT(changePageGeometry(GGViewPage*,QRect)));

    GGAbstractCommandFactory::oneShotCommand(fac->createStartPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createEndPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createConditionPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createActionPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createDecisionPage(QRect(0,0,100,100)));
    delete fac;

    QAction *a = new QAction(this);
    a->setShortcut(QKeySequence::Undo);
    connect(a, SIGNAL(triggered()), m_ctrl, SLOT(undo()));
    addAction(a);

    a = new QAction(this);
    a->setShortcut(QKeySequence::Redo);
    connect(a, SIGNAL(triggered()), m_ctrl, SLOT(redo()));
    addAction(a);
}
