#include "tst_ggeditor_viewcommandtest.h"
#include <viewcommand/ggviewcommands.h>
#include <viewcommand/ggviewcommandfactory.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewscene.h>
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
    m_s = m_em->factory()->createScene();
    m_em->registerNewScene(m_s);
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
    GGCreateViewPageCmd *cmd = m_vf->createStartPage(m_s, QRect(0, 0, 1 ,1));

    QVERIFY2(cmd->execute(), "Cannot execute create view page");
    VERIFYVSIG(m_vmsc, "Create View Page CMD", 0,0, 1,0, 0,0, 0,1);
    GGPage *p = cmd->getInnerCommand()->createdPage();
    QVERIFY2(m_em->getPages().size() == 1, "No page created");
    QVERIFY2(m_em->getPage(p->id()) == p, "Create page is not the page from the command");
    QVERIFY2(m_vm->getViewPageForPage(p, m_s->id()) != NULL, "No View Page Created");
    QVERIFY2(m_vm->getViewPageForPage(p, m_s->id())->page() == p, "View Page not associated with page");
    GGViewPage *vp = m_vm->getViewPageForPage(p, m_s->id());
    QVERIFY2(!vp->bounds().isEmpty(), "Bounds not set");

    QVERIFY2(cmd->undo(), "Cannot undo create view page");
    VERIFYVSIG(m_vmsc, "Undo Create View Page CMD", 0,0, 0,1, 0,0, 0,0);
    QVERIFY2(m_em->getPages().isEmpty(), "Page still there after undo create view page");
    //QVERIFY2(vm.getViewPageForPage(p) == vp, "View Page lost before processing commands after undo create view page");
    //qApp->processEvents();
    QVERIFY2(m_vm->getViewPageForPage(p, m_s->id()) == NULL, "View Page still there after undo create view page");

    QVERIFY2(cmd->redo(), "Cannot redo create view page");
    VERIFYVSIG(m_vmsc, "Redo Create View Page CMD", 0,0, 1,0, 0,0, 0,0);
    //QVERIFY2(vm.getViewPageForPage(p) == NULL, "View Page already there before processing commands after redo create view page");
    //qApp->processEvents();
    QVERIFY2(m_vm->getViewPageForPage(p, m_s->id()) != NULL, "View Page lost after processing commands after redo create view page");
    QVERIFY2(m_vm->getViewPageForPage(p, m_s->id()) == vp, "View Page changed (not recycled) after redoing create view page");
    VERIFYVSIGNULL(m_vmsc, "After Create");

    delete cmd;
}

void GGEditor_ViewCommandTest::testCreateViewConnection()
{
    QVERIFY(m_stk->execute(m_vf->createStartPage(m_s, QRect(0,0,1,1))));
    QVERIFY(m_stk->execute(m_vf->createEndPage(m_s, QRect(10,10,1,1))));
    VERIFYVSIG(m_vmsc, "Create pages", 0,0, 2, 0, 0, 0, 0, 2);

    GGViewPage *s = m_vm->getViewPageForPage(m_em->getPages()[0], m_s->id());
    GGViewPage *e = m_vm->getViewPageForPage(m_em->getPages()[1], m_s->id());
    QVERIFY2(m_stk->execute(m_vf->createConnection(s, e, GGConnectionSlot::StartConnection)), "Cannot create view connection");
    VERIFYVSIG(m_vmsc, "Create Connection", 0,0, 0, 0, 1, 0, 1, 0);
    QVERIFY2(m_em->getConnections().size() == 1, "Connection not set in model");
    GGConnection *c = m_em->getConnections()[0];
    QVERIFY2(GG::as<GGStartPage>(s->page())->startConnection() == c, "Connection not set in page");
    QVERIFY2(m_vm->getViewConnectionForConnection(c, m_s->id()) != NULL, "Cannot get ViewConnection for Connection");
    GGViewConnection *vc = m_vm->getViewConnectionForConnection(c, m_s->id());
    QVERIFY2(vc->connection() == c, "Connection not set in ViewConnection");

    QVERIFY2(m_stk->undo(), "Cannot undo create connection");
    VERIFYVSIG(m_vmsc, "Undo Create View Connection", 0,0, 0, 0, 0, 1, 1, 0);
    QVERIFY2(m_em->getConnections().isEmpty(), "Connection not removed");
    QVERIFY2(m_vm->getViewConnectionForConnection(c, m_s->id()) == NULL, "Can still get viewconnection for Connetion after undo");

    QVERIFY2(m_stk->redo(), "Cannot redo create connection");
    VERIFYVSIG(m_vmsc, "Redo Create Connection", 0,0, 0, 0, 1, 0, 1, 0);
}

void GGEditor_ViewCommandTest::testDeleteViewPage()
{
    QVERIFY(m_stk->execute(m_vf->createStartPage(m_s, QRect(0,0,1,1))));
    QVERIFY(m_stk->execute(m_vf->createEndPage(m_s, QRect(10,10,1,1))));
    QVERIFY(m_stk->execute(m_vf->createConditionPage(m_s, QRect(20,20,1,1))));
    VERIFYVSIG(m_vmsc, "Create pages", 0,0, 3, 0, 0, 0, 0, 3);

    GGViewPage *s = m_vm->getViewPageForPage(m_em->getPages()[0], m_s->id());
    GGViewPage *e = m_vm->getViewPageForPage(m_em->getPages()[1], m_s->id());
    GGViewPage *d = m_vm->getViewPageForPage(m_em->getPages()[2], m_s->id());

    QVERIFY(m_stk->execute(m_vf->createConnection(s, d, GGConnectionSlot::StartConnection)));
    QVERIFY(m_stk->execute(m_vf->createConnection(d, e, GGConnectionSlot::TrueConnection)));
    VERIFYVSIG(m_vmsc, "Create connections", 0,0, 0, 0, 2, 0, 2, 0);

    GGViewConnection *c1 = m_vm->getViewConnectionForConnection(m_em->getConnections()[0], m_s->id());
    GGViewConnection *c2 = m_vm->getViewConnectionForConnection(m_em->getConnections()[1], m_s->id());

    QVERIFY2(m_stk->execute(m_vf->deletePage(d)), "Cannot delete Page");
    VERIFYVSIG(m_vmsc, "Delete page", 0,0, 0, 1, 0, 2, 1, 0);
    QVERIFY(m_em->getPageIncommingConnections(e->page()).isEmpty());

    QVERIFY2(m_stk->undo(), "Cannot undo delete page");
    VERIFYVSIG(m_vmsc, "Undo delete page", 0,0, 1, 0, 2, 0, 2, 0);
    QVERIFY2(m_vm->getViewConnectionForConnection(m_em->getConnections()[0], m_s->id()) == c1, "View Connection changed after undo delete page");
    QVERIFY2(m_vm->getViewConnectionForConnection(m_em->getConnections()[1], m_s->id()) == c2, "View Connection changed after undo delete page");

    QVERIFY2(m_stk->redo(), "Cannot redo delete page");
    VERIFYVSIG(m_vmsc, "Redo Delete page", 0,0, 0, 1, 0, 2, 1, 0);
}

void GGEditor_ViewCommandTest::testViewScenes()
{
    QVERIFY2(m_stk->execute(m_vf->createScene("2nd", "2nd")), "Cannot create 2nd scene");
    VERIFYVSIG(m_vmsc, "Create 2nd scene", 1,0, 0,0, 0,0, 0,0);
    GGScene *sc2 = m_em->getScenes()[1];

    QVERIFY(m_vm->getViewSceneForScene(sc2));
    QVERIFY(sc2 == m_vm->getViewSceneForScene(sc2)->scene());
    QVERIFY(m_vm->getViewSceneForScene(m_s));
    QVERIFY(m_s == m_vm->getViewSceneForScene(m_s)->scene());

    QVERIFY2(m_stk->undo(), "Undo create scene");
    VERIFYVSIG(m_vmsc, "Undo create scene", 0,1, 0,0, 0,0, 0,0);
    QVERIFY(m_em->getScenes().size() == 1);
    QVERIFY(m_em->getScene(sc2->id()) == NULL);
    QVERIFY2(m_vm->getViewSceneForScene(sc2) == NULL, "View scene still there after undo create");
    QVERIFY2(m_stk->redo(), "Redo create scene");
    VERIFYVSIG(m_vmsc, "Redo create scene", 1,0, 0,0, 0,0, 0,0);
    QVERIFY(m_em->getScenes().size() == 2);
    QVERIFY(m_em->getScene(sc2->id()) != NULL);
    QVERIFY(m_vm->getViewSceneForScene(sc2) != NULL);
    QVERIFY(m_vm->getViewSceneForScene(sc2)->scene() == sc2);


    QVERIFY2(m_stk->execute(m_vf->deleteScene(sc2)), "Cannot delete empty scene");
    VERIFYVSIG(m_vmsc, "Delete scene", 0,1, 0,0, 0,0, 0,0);
    QVERIFY(m_em->getScenes().size() == 1);
    QVERIFY(m_em->getScene(sc2->id()) == NULL);
    QVERIFY2(m_vm->getViewSceneForScene(sc2) == NULL, "View scene still there after delete");
    QVERIFY2(m_stk->undo(), "Cannot undo delete scene");
    VERIFYVSIG(m_vmsc, "Undo Delete scene", 1,0, 0,0, 0,0, 0,0);
    QVERIFY(m_em->getScenes().size() == 2);
    QVERIFY(m_em->getScene(sc2->id()) != NULL);
    QVERIFY(m_vm->getViewSceneForScene(sc2) != NULL);
    QVERIFY(m_vm->getViewSceneForScene(sc2)->scene() == sc2);
    QVERIFY2(m_stk->redo(), "Cannot redo delete scene");
    VERIFYVSIG(m_vmsc, "Redo Delete scene", 0,1, 0,0, 0,0, 0,0);
    QVERIFY(m_em->getScenes().size() == 1);
    QVERIFY(m_em->getScene(sc2->id()) == NULL);
    QVERIFY2(m_vm->getViewSceneForScene(sc2) == NULL, "View scene still there after delete");

    // Required for tests below
    QVERIFY(m_stk->undo());
    QVERIFY(m_em->getScenes().size() == 2);
    QVERIFY(m_em->getScenes().contains(m_s));
    QVERIFY(m_em->getScenes().contains(sc2));
    m_vmsc->reset();

    QVERIFY(m_stk->execute(m_vf->createStartPage(m_s, QRect())));
    QVERIFY(m_stk->execute(m_vf->createEndPage(sc2, QRect())));
    VERIFYVSIG(m_vmsc, "Register pages in different scenes", 0,0, 2,0, 0,0, 0,2);
    QVERIFY(m_s->pages().contains(m_em->getPages()[0]));
    QVERIFY(sc2->pages().contains(m_em->getPages()[1]));

}

void GGEditor_ViewCommandTest::testViewScenePages()
{
    QVERIFY(m_stk->execute(m_vf->createScene("default", "default")));
    GGScene *sc2 = m_em->getScenes()[1];

    QVERIFY(m_stk->execute(m_vf->createConditionPage(m_s, QRect())));
    QVERIFY(m_stk->execute(m_vf->createEndPage(sc2, QRect())));
    QVERIFY(m_stk->execute(m_vf->createEndPage(sc2, QRect())));
    QVERIFY(m_stk->execute(m_vf->createEndPage(m_s, QRect())));
    GGConditionPage *s1p1 = GG::as<GGConditionPage> (m_em->getPages()[0]);
    GGPage *s2p1 = m_em->getPages()[1];
    GGPage *s2p2 = m_em->getPages()[2];
    GGPage *s1p2 = m_em->getPages()[3];
    VERIFYVSIG(m_vmsc, "", 1,0, 4,0, 0,0, 0,4);
    QVERIFY(m_s->pages().contains(s1p1));
    QVERIFY(m_s->pages().contains(s1p2));
    QVERIFY(sc2->pages().contains(s2p1));
    QVERIFY(sc2->pages().contains(s2p2));

    QVERIFY2(m_vm->getViewPageForPage(s1p1, m_s->id()), "Cannot get viewpage in scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p2, m_s->id()), "Cannot get viewpage in scene");
    QVERIFY2(m_vm->getViewPageForPage(s2p1, sc2->id()), "Cannot get viewpage in scene");
    QVERIFY2(m_vm->getViewPageForPage(s2p1, sc2->id()), "Cannot get viewpage in scene");

    QVERIFY2(m_vm->getViewPageForPage(s2p1, m_s->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY2(m_vm->getViewPageForPage(s2p2, m_s->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()) == NULL, "Can get viewpage in wrong scene");

    // Create in scene connection
    QVERIFY(m_stk->execute(m_vf->createConnection(m_vm->getViewPageForPage(s1p1, m_s->id()), m_vm->getViewPageForPage(s1p2, m_s->id()), GGConnectionSlot::TrueConnection)));
    VERIFYVSIG(m_vmsc, "Create in-scene connection", 0,0, 0,0, 1,0, 1,0);
    QVERIFY2(m_vm->getViewPageForPage(s2p1, m_s->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY2(m_vm->getViewPageForPage(s2p2, m_s->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()) == NULL, "Can get viewpage in wrong scene");
    QVERIFY(m_stk->undo());
    m_vmsc->reset();

    // Create cross-scene connection
    QVERIFY(m_stk->execute(m_vf->createConnection(m_vm->getViewPageForPage(s1p1, m_s->id()), m_vm->getViewPageForPage(s2p1, sc2->id()), GGConnectionSlot::TrueConnection)));
    GGConnection *c1 = s1p1->trueConnection();
    QVERIFY2(m_vm->getViewConnectionForConnection(c1, m_s->id()), "Connection not in source scene");
    QVERIFY2(m_vm->getViewConnectionForConnection(c1, sc2->id()), "Connection not in dest scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()), "Source page not in dest scene");
    QVERIFY2(m_vm->getViewPageForPage(s2p1, m_s->id()), "Destination page not in source scene");
    // As page updates are done for EVERY view page, we get 2 changes!
    VERIFYVSIG(m_vmsc, "Create cross-scene connection", 0,0, 2,0, 2,0, 2,0);

    QVERIFY(m_stk->undo());
    QVERIFY2(m_vm->getViewConnectionForConnection(c1, m_s->id()) == NULL, "Connection still in source scene after removing connection");
    QVERIFY2(m_vm->getViewConnectionForConnection(c1, sc2->id()) == NULL, "Connection still in dest scene after removing connection");
    QVERIFY2(m_vm->getViewPageForPage(s2p1, m_s->id()) == NULL, "Dest page still in source scene after removing connection");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()) == NULL, "Source page still in dest scene after removing connection");
    // As page updates are done for EVERY view page and are done before removing the view page, we get 2 changes!
    // This might be a problem, as updates for unregistering pages occur, but is not fixed now
    QWARN("Cross-scene connection removal signals updates to unregistering view pages");
    VERIFYVSIG(m_vmsc, "Undo Cross-scene conn", 0,0, 0,2, 0,2, 2,0);

    QVERIFY(m_stk->redo());
    m_vmsc->reset();

    // Create 2nd cross-scene connection
    QVERIFY(m_stk->execute(m_vf->createConnection(m_vm->getViewPageForPage(s1p1, m_s->id()), m_vm->getViewPageForPage(s2p2, sc2->id()), GGConnectionSlot::FalseConnection)));
    GGConnection *c2 = s1p1->falseConnection();
    QVERIFY2(m_vm->getViewConnectionForConnection(c2, m_s->id()), "Connection not in source scene");
    QVERIFY2(m_vm->getViewConnectionForConnection(c2, sc2->id()), "Connection not in dest scene");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()), "Source page not in dest scene");
    QVERIFY2(m_vm->getViewPageForPage(s2p1, m_s->id()), "Destination page not in source scene");
    // As page updates are done for EVERY view page, we get 2 changes!
    VERIFYVSIG(m_vmsc, "Create cross-scene connection", 0,0, 1,0, 2,0, 2,0);

    QVERIFY(m_stk->undo());
    QVERIFY2(m_vm->getViewConnectionForConnection(c2, m_s->id()) == NULL, "Connection still in source scene after removing connection");
    QVERIFY2(m_vm->getViewConnectionForConnection(c2, sc2->id()) == NULL, "Connection still in dest scene after removing connection");
    QVERIFY2(m_vm->getViewPageForPage(s1p1, sc2->id()), "Source page no longer in dest scene after removing 2nd connection");
    QVERIFY2(m_vm->getViewPageForPage(s2p2, m_s->id()) == NULL, "Destination page still in source scene");
    // As page updates are done for EVERY view page, we get 2 changes!
    VERIFYVSIG(m_vmsc, "Remove 2nd cross-scene connection", 0,0, 0,1, 0,2, 2,0);
}

