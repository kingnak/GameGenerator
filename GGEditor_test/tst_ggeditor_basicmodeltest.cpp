#include "tst_ggeditor_basicmodeltest.h"

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
    s = model->factory()->createStartPage();
    e = model->factory()->createEndPage();

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
    delete model;
}

void GGEditor_BasicModelTest::cleanupTestCase()
{
}

void GGEditor_BasicModelTest::testRegister()
{
    QVERIFY2(model->getPage(s->id()) == s, "getPage(s->id()) is not s");
    QVERIFY2(model->getPage(e->id()) == e, "getPage(e->id()) is not e");
    QVERIFY2(model->getPage(GG::InvalidPageId) == NULL, "getPage(GG::InvalidPageId) is not NULL");

    QVERIFY2(model->getConnection(c->id()) == c, "getConnection(c->id()) is not c");

    QVERIFY2(s->getConnections().size() == 1, "Start has wrong # of connections");
    QVERIFY2(s->getConnections().value(0) == c, "Start has not c as connections");

    QVERIFY2(c->source() == s, "Source is not s");
    QVERIFY2(c->destination() == e, "Destination is not e");
    QVERIFY2(model->getPageIncommingConnections(s).size() == 0, "s has incomming connections");
    QVERIFY2(model->getPageIncommingConnections(e).size() == 1, "e has wrong # of incomming connections");
    QVERIFY2(model->getPageIncommingConnections(e).contains(c), "incomming connection for e is not c");
}

void GGEditor_BasicModelTest::testUnregisterStart()
{
    QList<GGConnection *> a;
    QVERIFY2(model->unregisterPage(s->id(), &a), "Error in unregister");
    QVERIFY2(a.size() == 1, "Wrong # of affected connections");
    QVERIFY2(a.value(0) == c, "c is not affected");
    QVERIFY2(s->id() != GG::InvalidPageId, "s->id() is invalid");
    QVERIFY2(e->id() != GG::InvalidPageId, "e->id() is invalid");
    QVERIFY2(c->id() != GG::InvalidConnectionId, "c->id() is invalid");
    QVERIFY2(model->getPage(s->id()) == NULL, "s still in model");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model");
    QVERIFY2(model->getPageIncommingConnections(e).size() == 0, "e has incomming connections");
}

void GGEditor_BasicModelTest::testUnregisterEnd()
{
    QList<GGConnection *> a;
    QVERIFY2(model->unregisterPage(e->id(), &a), "Error in unregister");
    QVERIFY2(a.size() == 1, "Wrong # of affected connections");
    QVERIFY2(a.value(0) == c, "c is not affected");
    QVERIFY2(s->id() != GG::InvalidPageId, "s->id() is invalid");
    QVERIFY2(e->id() != GG::InvalidPageId, "e->id() is invalid");
    QVERIFY2(c->id() != GG::InvalidConnectionId, "c->id() is invalid");
    QVERIFY2(model->getPage(e->id()) == NULL, "e still in model");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model");

    QVERIFY2(s->getConnections().isEmpty(), "s still has connection");

}

void GGEditor_BasicModelTest::testUnregisterConnection()
{
    QVERIFY2(model->unregisterConnection(c->id()), "Error in unregister");
    QVERIFY2(s->id() != GG::InvalidPageId, "s->id() is invalid");
    QVERIFY2(e->id() != GG::InvalidPageId, "e->id() is invalid");
    QVERIFY2(c->id() != GG::InvalidConnectionId, "c->id() is invalid");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model");
    QVERIFY2(model->getPageIncommingConnections(e).size() == 0, "e has incomming connections");

    QVERIFY2(s->getConnections().isEmpty(), "s still has connection");
}

void GGEditor_BasicModelTest::testReregister()
{
    // reregister s
    model->unregisterPage(s->id());
    QVERIFY2(model->registerPageWithId(s), "Error in re-register s");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model after reregistering s");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model after reregistering s");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model after reregistering s");

    // reregister c
    QVERIFY2(model->registerConnectionWithId(c), "Error in re-register c");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model after reregistering c");
    QVERIFY2(model->getConnection(c->id()) == c, "c is no longer in model after reregistering c");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model after reregistering c");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().size() == 1, "s has no connection after reregistering c");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().value(0) == c, "s has not c after reregistering c");
    // HACK
    ((GGStartPage*)s)->setStartConnection(c);

    // reregister e
    model->unregisterPage(e->id());
    QVERIFY2(model->registerPageWithId(e), "Error in re-register e");
    QVERIFY2(model->getPage(e->id()) == e, "e is no longer in the model after reregistering e");
    QVERIFY2(model->getConnection(c->id()) == NULL, "c is still in model after reregistering e");
    QVERIFY2(model->getPage(s->id()) == s, "s is no longer in the model after reregistering e");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().size() == 1, "s has no connection after reregistering c");
    QEXPECT_FAIL("", "Setting connections on pages will be done by Commands", Continue);
    QVERIFY2(s->getConnections().value(0) == c, "s has not c after reregistering c");
    // HACK
    ((GGStartPage*)s)->setStartConnection(c);
}
