#include "tst_ggeditor_viewcommandtest.h"
#include <viewcommand/ggviewcommands.h>
#include <viewcommand/ggviewcommandfactory.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <viewmodel/ggviewconnection.h>
#include <command/ggcommandstack.h>
#include "testsignalchecker.h"

GGEditor_ViewCommandTest::GGEditor_ViewCommandTest(QObject *parent) : QObject(parent)
{

}

void GGEditor_ViewCommandTest::init()
{
    m_em = new GGEditModel(new GGSimpleFactory);
    m_vm = new GGViewModel(m_em);
    m_vf = new GGViewCommandFactory(m_vm);
    m_vmsc = new ViewModelSignalChecker(m_vm);
    m_stk = new GGCommandStack;
}

void GGEditor_ViewCommandTest::cleanup()
{
    delete m_stk;
    delete m_vmsc;
    delete m_vf;
    delete m_vm;
    delete m_em;
}

void GGEditor_ViewCommandTest::testCreateViewPage()
{

    GGCreateViewPageCmd *cmd = m_vf->createStartPage(QRect(0, 0, 1 ,1));

    QVERIFY2(cmd->execute(), "Cannot execute create view page");
    VERIFYVSIG(m_vmsc, "Create View Page CMD", 1, 0, 0, 0, 0, 1);
    GGPage *p = cmd->getInnerCommand()->createdPage();
    QVERIFY2(m_em->getPages().size() == 1, "No page created");
    QVERIFY2(m_em->getPage(p->id()) == p, "Create page is not the page from the command");
    QVERIFY2(m_vm->getViewPageForPage(p) != NULL, "No View Page Created");
    QVERIFY2(m_vm->getViewPageForPage(p)->page() == p, "View Page not associated with page");
    GGViewPage *vp = m_vm->getViewPageForPage(p);
    QVERIFY2(!vp->bounds().isEmpty(), "Bounds not set");

    QVERIFY2(cmd->undo(), "Cannot undo create view page");
    VERIFYVSIG(m_vmsc, "Undo Create View Page CMD", 0, 1, 0, 0, 0, 0);
    QVERIFY2(m_em->getPages().isEmpty(), "Page still there after undo create view page");
    //QVERIFY2(vm.getViewPageForPage(p) == vp, "View Page lost before processing commands after undo create view page");
    //qApp->processEvents();
    QVERIFY2(m_vm->getViewPageForPage(p) == NULL, "View Page still there after undo create view page");

    QVERIFY2(cmd->redo(), "Cannot redo create view page");
    VERIFYVSIG(m_vmsc, "Redo Create View Page CMD", 1, 0, 0, 0, 0, 0);
    //QVERIFY2(vm.getViewPageForPage(p) == NULL, "View Page already there before processing commands after redo create view page");
    //qApp->processEvents();
    QVERIFY2(m_vm->getViewPageForPage(p) != NULL, "View Page lost after processing commands after redo create view page");
    QVERIFY2(m_vm->getViewPageForPage(p) == vp, "View Page changed (not recycled) after redoing create view page");
    VERIFYVSIGNULL(m_vmsc, "After Create");

    delete cmd;
}

void GGEditor_ViewCommandTest::testCreateViewConnection()
{
    QVERIFY(m_stk->execute(m_vf->createStartPage(QRect(0,0,1,1))));
    QVERIFY(m_stk->execute(m_vf->createEndPage(QRect(10,10,1,1))));
    VERIFYVSIG(m_vmsc, "Create pages", 2, 0, 0, 0, 0, 2);

    GGViewPage *s = m_vm->getViewPageForPage(m_em->getPages()[0]);
    GGViewPage *e = m_vm->getViewPageForPage(m_em->getPages()[1]);
    QVERIFY2(m_stk->execute(m_vf->createConnection(s, e, GGConnectionSlot::StartConnection)), "Cannot create view connection");
    VERIFYVSIG(m_vmsc, "Create Connection", 0, 0, 1, 0, 1, 0);
    QVERIFY2(m_em->getConnections().size() == 1, "Connection not set in model");
    GGConnection *c = m_em->getConnections()[0];
    QVERIFY2(GG::as<GGStartPage>(s->page())->startConnection() == c, "Connection not set in page");
    QVERIFY2(m_vm->getViewConectionForConnection(c) != NULL, "Cannot get ViewConnection for Connection");
    GGViewConnection *vc = m_vm->getViewConectionForConnection(c);
    QVERIFY2(vc->connection() == c, "Connection not set in ViewConnection");

    QVERIFY2(m_stk->undo(), "Cannot undo create connection");
    VERIFYVSIG(m_vmsc, "Undo Create View Connection", 0, 0, 0, 1, 1, 0);
    QVERIFY2(m_em->getConnections().isEmpty(), "Connection not removed");
    QVERIFY2(m_vm->getViewConectionForConnection(c) == NULL, "Can still get viewconnection for Connetion after undo");

    QVERIFY2(m_stk->redo(), "Cannot redo create connection");
    VERIFYVSIG(m_vmsc, "Redo Create Connection", 0, 0, 1, 0, 1, 0);
}

void GGEditor_ViewCommandTest::testDeleteViewPage()
{
    QVERIFY(m_stk->execute(m_vf->createStartPage(QRect(0,0,1,1))));
    QVERIFY(m_stk->execute(m_vf->createEndPage(QRect(10,10,1,1))));
    QVERIFY(m_stk->execute(m_vf->createConditionPage(QRect(20,20,1,1))));
    VERIFYVSIG(m_vmsc, "Create pages", 3, 0, 0, 0, 0, 3);

    GGViewPage *s = m_vm->getViewPageForPage(m_em->getPages()[0]);
    GGViewPage *e = m_vm->getViewPageForPage(m_em->getPages()[1]);
    GGViewPage *d = m_vm->getViewPageForPage(m_em->getPages()[2]);

    QVERIFY(m_stk->execute(m_vf->createConnection(s, d, GGConnectionSlot::StartConnection)));
    QVERIFY(m_stk->execute(m_vf->createConnection(d, e, GGConnectionSlot::TrueConnection)));
    VERIFYVSIG(m_vmsc, "Create connections", 0, 0, 2, 0, 2, 0);

    GGViewConnection *c1 = m_vm->getViewConectionForConnection(m_em->getConnections()[0]);
    GGViewConnection *c2 = m_vm->getViewConectionForConnection(m_em->getConnections()[1]);

    QVERIFY2(m_stk->execute(m_vf->deletePage(d)), "Cannot delete Page");
    VERIFYVSIG(m_vmsc, "Delete page", 0, 1, 0, 2, 1, 0);
    QVERIFY(m_em->getPageIncommingConnections(e->page()).isEmpty());

    QVERIFY2(m_stk->undo(), "Cannot undo delete page");
    VERIFYVSIG(m_vmsc, "Undo delete page", 1, 0, 2, 0, 2, 0);
    QVERIFY2(m_vm->getViewConectionForConnection(m_em->getConnections()[0]) == c1, "View Connection changed after undo delete page");
    QVERIFY2(m_vm->getViewConectionForConnection(m_em->getConnections()[1]) == c2, "View Connection changed after undo delete page");

    QVERIFY2(m_stk->redo(), "Cannot redo delete page");
    VERIFYVSIG(m_vmsc, "Redo Delete page", 0, 1, 0, 2, 1, 0);
}

