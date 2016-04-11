#include "tst_ggeditor_basicmodeltest.h"
#include "testsignalchecker.h"
#include <model/ggscene.h>
#include <model/ggmediaresolver.h>

GGEditor_BasicModelTest::GGEditor_BasicModelTest()
{
}

void GGEditor_BasicModelTest::initTestCase()
{
    m_warnedAboutConnectionHack = false;
}

void GGEditor_BasicModelTest::init()
{
    model = new GGEditModel(new GGSimpleFactory);
    sc = new ModelSignalChecker(model);
    scn = model->factory()->createScene();
    model->registerNewScene(scn);

    s = model->factory()->createStartPage(scn->id());
    e = model->factory()->createEndPage(scn->id());

    model->registerNewPage(s);
    model->registerNewPage(e);
    c = model->factory()->createConnection(s->id(), e->id());
    model->registerNewConnection(c);

    // This is a "hack" here. In future, this should be done by Commands
    if (!m_warnedAboutConnectionHack) QWARN("s->c is set manually. In future, this must be done by commands!");
    m_warnedAboutConnectionHack = true;
    ((GGStartPage*)s)->setStartConnection(c);
}

void GGEditor_BasicModelTest::cleanup()
{
    delete sc;
    delete model;
}

void GGEditor_BasicModelTest::cleanupTestCase()
{
}

void GGEditor_BasicModelTest::testRegister()
{
    VERIFYSIG(sc, "Initial Model", 1, 0, 2, 0, 1, 0, 1);

    QVERIFY2(model->getScene(scn->id()) == scn, "getScene(scn->id()) is not scn");
    QVERIFY2(model->getScene(GG::InvalidSceneId) == NULL, "getScene(GG::InvalidSceneId) is not NULL");

    QVERIFY2(model->getPage(s->id()) == s, "getPage(s->id()) is not s");
    QVERIFY2(model->getPage(e->id()) == e, "getPage(e->id()) is not e");
    QVERIFY2(model->getPage(GG::InvalidPageId) == NULL, "getPage(GG::InvalidPageId) is not NULL");

    QVERIFY2(scn->pages().size() == 2, "Scene has wrong # of pages");
    QVERIFY2(scn->pages().contains(s), "Scene does not contain start");
    QVERIFY2(scn->pages().contains(e), "Scene does not contain end");

    QVERIFY2(model->getConnection(c->id()) == c, "getConnection(c->id()) is not c");

    QVERIFY2(s->getConnections().size() == 1, "Start has wrong # of connections");
    QVERIFY2(s->getConnections().value(0) == c, "Start has not c as connections");

    QVERIFY2(c->source() == s, "Source is not s");
    QVERIFY2(c->destination() == e, "Destination is not e");
    QVERIFY2(model->getPageIncommingConnections(s).size() == 0, "s has incomming connections");
    QVERIFY2(model->getPageIncommingConnections(e).size() == 1, "e has wrong # of incomming connections");
    QVERIFY2(model->getPageIncommingConnections(e).contains(c), "incomming connection for e is not c");

    VERIFYSIGNULL(sc, "After initial Model checks");
}

void GGEditor_BasicModelTest::testUnregisterStart()
{
    QList<GGConnection *> a;
    sc->reset();
    QVERIFY2(model->unregisterPage(s->id(), &a), "Error in unregister");
    // also unregistered s->e connection, and removed from s
    VERIFYSIG(sc, "Unregister start", 0,0,0,1,0,1,0);

    QVERIFY2(a.size() == 1, "Wrong # of affected connections");
    QVERIFY2(a.value(0) == c, "c is not affected");
    QVERIFY2(s->id() != GG::InvalidPageId, "s->id() is invalid");
    QVERIFY2(e->id() != GG::InvalidPageId, "e->id() is invalid");
    QVERIFY2(c->id() != GG::InvalidConnectionId, "c->id() is invalid");
    QVERIFY2(model->getPage(s->id()) == NULL, "s still in model");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model");
    QVERIFY2(model->getPageIncommingConnections(e).size() == 0, "e has incomming connections");

    VERIFYSIGNULL(sc, "After unregister Start");

    // clean memory
    delete s;
    qDeleteAll(a);
}

void GGEditor_BasicModelTest::testUnregisterEnd()
{
    sc->reset();
    QList<GGConnection *> a;
    QVERIFY2(model->unregisterPage(e->id(), &a), "Error in unregister");
    // Start page is modified
    VERIFYSIG(sc, "Unregister end", 0, 0, 0, 1, 0, 1, 1);
    QVERIFY2(a.size() == 1, "Wrong # of affected connections");
    QVERIFY2(a.value(0) == c, "c is not affected");
    QVERIFY2(s->id() != GG::InvalidPageId, "s->id() is invalid");
    QVERIFY2(e->id() != GG::InvalidPageId, "e->id() is invalid");
    QVERIFY2(c->id() != GG::InvalidConnectionId, "c->id() is invalid");
    QVERIFY2(model->getPage(e->id()) == NULL, "e still in model");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model");

    QVERIFY2(s->getConnections().isEmpty(), "s still has connection");

    VERIFYSIGNULL(sc, "After unregister End");

    // clean memory
    delete e;
    qDeleteAll(a);
}

void GGEditor_BasicModelTest::testUnregisterConnection()
{
    sc->reset();
    QVERIFY2(model->unregisterConnection(c->id()), "Error in unregister");
    VERIFYSIG(sc, "After unregister connection", 0 , 0, 0, 0, 0, 1, 1);
    QVERIFY2(s->id() != GG::InvalidPageId, "s->id() is invalid");
    QVERIFY2(e->id() != GG::InvalidPageId, "e->id() is invalid");
    QVERIFY2(c->id() != GG::InvalidConnectionId, "c->id() is invalid");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model");
    QVERIFY2(model->getPageIncommingConnections(e).size() == 0, "e has incomming connections");

    QVERIFY2(s->getConnections().isEmpty(), "s still has connection");

    VERIFYSIGNULL(sc, "After unregister Connection");

    // clean memory
    delete c;
}

void GGEditor_BasicModelTest::testReregister()
{
    sc->reset();
    // reregister s
    model->unregisterPage(s->id());
    VERIFYSIG(sc, "", 0, 0, 0, 1, 0, 1, 0);
    QVERIFY2(model->registerPageWithId(s), "Error in re-register s");
    // Connections are not re-registered. Commands are used for that
    VERIFYSIG(sc, "Re-register page", 0, 0, 1, 0, 0, 0, 0);
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model after reregistering s");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model after reregistering s");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model after reregistering s");

    // reregister c
    QVERIFY2(model->registerConnectionWithId(c), "Error in re-register c");
    // Connection is not assigned. Commands are used for that
    VERIFYSIG(sc, "Re-register connection", 0, 0, 0, 0, 1, 0, 0);
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model after reregistering c");
    QVERIFY2(model->getConnection(c->id()) == c, "c is no longer in model after reregistering c");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model after reregistering c");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().size() == 1, "s has no connection after reregistering c");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().value(0) == c, "s has not c after reregistering c");
    // HACK
    ((GGStartPage*)s)->setStartConnection(c);
    VERIFYSIG(sc, "Setting connection", 0, 0, 0, 0, 0, 0, 1);

    // reregister e
    model->unregisterPage(e->id());
    // Start page is modified
    VERIFYSIG(sc, "", 0, 0, 0, 1, 0, 1, 1);
    QVERIFY2(model->registerPageWithId(e), "Error in re-register e");
    VERIFYSIG(sc, "Re-register page", 0, 0, 1, 0, 0, 0, 0);
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model after reregistering e");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model after reregistering e");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model after reregistering e");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().size() == 1, "s has no connection after reregistering c");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().value(0) == c, "s has not c after reregistering c");
    QVERIFY2(model->registerConnectionWithId(c), "Error in reregistering c after reregistering e");
    VERIFYSIG(sc, "Re-register connection", 0, 0, 0, 0, 1, 0, 0);
    // HACK
    ((GGStartPage*)s)->setStartConnection(c);
    VERIFYSIG(sc, "Set connection", 0, 0, 0, 0, 0, 0, 1);
}

void GGEditor_BasicModelTest::testScenes()
{
    sc->reset();
    GGScene *scn2 = model->factory()->createScene();
    QVERIFY(model->registerNewScene(scn2));
    VERIFYSIG(sc, "Register 2nd scene", 1,0, 0,0, 0,0, 0);
    QVERIFY2(!model->unregisterScene(scn->id()), "Can unregister non-empty scene");
    VERIFYSIGNULL(sc, "Remove non-empty scene");
    QVERIFY(scn->pages().size() == 2);
    QVERIFY(scn2->pages().isEmpty());
    QVERIFY(model->unregisterScene(scn2->id()));
    VERIFYSIG(sc, "Unregister 2nd scene (empty)", 0,1, 0,0, 0,0, 0);
    QVERIFY2(model->registerSceneWithId(scn2), "Cannot reregister scene");
    VERIFYSIG(sc, "Reregister 2nd scene", 1,0, 0,0, 0,0, 0);

    QVERIFY2(!s->setSceneId(scn2->id()), "Can set scene on registered page");

    QList<GGConnection*> conList;
    model->unregisterPage(s->id(), &conList);
    VERIFYSIG(sc, "", 0,0, 0,1, 0,1, 0);
    QVERIFY2(scn->pages().size() == 1, "Page still in scene after unregister");

    QVERIFY2(s->setSceneId(scn2->id()), "Cannot set scene on unregistered page");
    QVERIFY2(model->registerPageWithId(s), "Cannot register page with changed scene");
    VERIFYSIG(sc, "After register of changed scene", 0,0, 1,0, 0,0, 0);
    QVERIFY2(scn->pages().size() == 1, "Page in wrong scene after re-register");
    QVERIFY2(scn2->pages().size() == 1, "Page not in correct scene");
    QVERIFY(scn->pages().contains(e));
    QVERIFY(scn2->pages().contains(s));

    QVERIFY2(model->registerConnectionWithId(conList[0]), "Cannot re-register connection after changing scene");
    // HACK
    ((GGStartPage*)s)->setStartConnection(conList[0]);
    VERIFYSIG(sc, "After reregister in scene", 0,0, 0,0, 1,0, 1);

    model->unregisterPage(e->id(), &conList);
    QVERIFY(scn->pages().isEmpty());
    QVERIFY(e->setSceneId(scn2->id()));
    QVERIFY(model->registerPageWithId(e));
    QVERIFY(scn->pages().isEmpty());
    QVERIFY(scn2->pages().size() == 2);
    QVERIFY(scn2->pages().contains(e));
    // HACK
    ((GGStartPage*)s)->setStartConnection(conList[0]);
    QVERIFY(model->registerConnectionWithId(conList[0]));

    VERIFYSIG(sc, "After moving 2nd page to other scene", 0,0, 1,1, 1,1, 2);
}
