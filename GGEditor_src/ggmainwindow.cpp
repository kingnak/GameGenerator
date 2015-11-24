#include "ggmainwindow.h"
#include <model/ggsimplefactory.h>
#include <model/ggeditmodel.h>
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
    GGEditModel *em = new GGEditModel(new GGSimpleFactory, this);
    GGViewModel *vm = new GGViewModel(em, this);
    m_ctrl = new GGUIController(this);

    GGEditorScene *sc = new GGEditorScene(this);
    sc->setSceneRect(-400,-400,800,800);
    QGraphicsView *v = new QGraphicsView(sc);
    setCentralWidget(v);
    v->setDragMode(QGraphicsView::RubberBandDrag);

    /*
    QGraphicsEllipseItem *e = new QGraphicsEllipseItem(-5,-5,10,10);
    e->setBrush(Qt::red);
    e->setZValue(100000);
    sc->addItem(e);
    */


    GGViewCommandFactory *fac = new GGViewCommandFactory(vm);
    GGAbstractCommandFactory::oneShotCommand(fac->createStartPage(QRect(-100,-100,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createEndPage(QRect(100,0,100,100)));
    GGPage *s = em->getPages()[0];
    GGAbstractCommandFactory::oneShotCommand(fac->createConditionPage(QRect(0,100,100,100)));
    GGPage *d = em->getPages()[1];
    GGAbstractCommandFactory::oneShotCommand(fac->createActionPage(QRect(100,100,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createDecisionPage(QRect(200,100,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createConnection(vm->getViewPageForPage(s), vm->getViewPageForPage(d), GGConnectionSlot::StartConnection));
    delete fac;

    sc->connectToController(m_ctrl);
    m_ctrl->setModel(vm);

    QAction *a = new QAction(this);
    a->setShortcut(QKeySequence::Undo);
    connect(a, SIGNAL(triggered()), m_ctrl, SLOT(undo()));
    addAction(a);

    a = new QAction(this);
    a->setShortcut(QKeySequence::Redo);
    connect(a, SIGNAL(triggered()), m_ctrl, SLOT(redo()));
    addAction(a);
}
