#include "ggmainwindow.h"
#include <model/ggsimplefactory.h>
#include <model/ggeditmodel.h>
#include <viewmodel/ggviewmodel.h>
#include <view/ggeditorscene.h>
#include <viewcommand/ggviewcommandfactory.h>
#include <viewcommand/ggviewcommands.h>
#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>

GGMainWindow::GGMainWindow()
{
    GGEditModel *em = new GGEditModel(new GGSimpleFactory, this);
    GGViewModel *vm = new GGViewModel(em, this);
    GGEditorScene *sc = new GGEditorScene(vm, this);
    QGraphicsView *v = new QGraphicsView(sc);
    setCentralWidget(v);
    GGViewCommandFactory *fac = new GGViewCommandFactory(vm);

    QGraphicsEllipseItem *e = new QGraphicsEllipseItem(-5,-5,10,10);
    e->setBrush(Qt::red);
    e->setZValue(100000);
    sc->addItem(e);
    //sc->addEllipse(0,0,3,3,QPen(Qt::yellow));

    //GGAbstractCommandFactory::oneShotCommand(fac->createStartPage(QRect(0,0,100,100)));
    //GGAbstractCommandFactory::oneShotCommand(fac->createEndPage(QRect(0,0,100,100)));
    //GGAbstractCommandFactory::oneShotCommand(fac->createConditionPage(QRect(0,0,100,100)));
    GGAbstractCommandFactory::oneShotCommand(fac->createActionPage(QRect(0,0,100,100)));
    //GGAbstractCommandFactory::oneShotCommand(fac->createDecisionPage(QRect(0,0,100,100)));
}
