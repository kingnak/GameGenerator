#include "tst_ggeditor_connectionslottest.h"

#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggconnectionslot.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <command/ggmodelpagecommands.h>
#include <command/ggmodelconnectioncommands.h>
#include <command/ggeditcommandfactory.h>
#include <command/ggcommandstack.h>

void GGEditor_ConnectionSlotTest::init()
{
    m_model = new GGEditModel(new GGSimpleFactory);
    m_cmdFac = new GGEditCommandFactory(m_model);
    //m_stack = new GGCommandStack;

    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createStartPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createEndPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createConditionPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createActionPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createDecisionPage());

}

void GGEditor_ConnectionSlotTest::cleanup()
{
    //delete m_stack;
    delete m_cmdFac;
    delete m_model;
}

void GGEditor_ConnectionSlotTest::testConnectionSlots()
{
    GGCommandStack stk;
    GGStartPage *s = ggpage_cast<GGStartPage*> (m_model->getPages()[0]);
    GGEndPage *e = ggpage_cast<GGEndPage*> (m_model->getPages()[1]);
    GGConditionPage *c = ggpage_cast<GGConditionPage*> (m_model->getPages()[2]);
    GGActionPage *a = ggpage_cast<GGActionPage*> (m_model->getPages()[3]);
    GGDecisionPage *d = ggpage_cast<GGDecisionPage*> (m_model->getPages()[4]);

    // Check correct start connection
    QVERIFY(stk.execute(m_cmdFac->createConnection(s, e, GGConnectionSlot::StartConnection)));
    GGConnection *cn = static_cast<GGCreateConnectionCmd *> (stk.getAllCommands()[0])->createdConnection();
    QVERIFY2(s->startConnection() == cn, "Connection not set in StartPage");
    QVERIFY2(m_model->getPageIncommingConnections(e).value(0) == cn, "Connection not incomming for start page");
    QVERIFY2(cn->source() == s && cn->destination() == e, "Src/Dest wrong for start page connection");
    QVERIFY(stk.undo());
    QVERIFY2(s->startConnection() == 0, "Connection not unset in StartPage");
    QVERIFY2(m_model->getPageIncommingConnections(e).size() == 0, "Connection still incomming for start page");
    QVERIFY(stk.redo());
    QVERIFY2(s->startConnection() == cn, "Connection not set in StartPage");
    QVERIFY2(m_model->getPageIncommingConnections(e).value(0) == cn, "Connection not incomming for start page");
    stk.undo();

    // Check wrong start connection
    QVERIFY2(!stk.execute(m_cmdFac->createConnection(s, e, GGConnectionSlot::ActionConnection)), "Can connect start page with wrong slot");
    QVERIFY2(s->startConnection() == 0, "Connection set in StartPage after using wrong slot");
    QVERIFY2(m_model->getPageIncommingConnections(e).size() == 0, "Connection incomming for start page after using wrong slot");

    // Check end page (cannot have outgoing)
    QVERIFY2(!stk.execute(m_cmdFac->createConnection(e, d, GGConnectionSlot::TrueConnection)), "Can connect end page");
    QVERIFY2(e->getConnections().isEmpty(), "EndPage has outgoing connection");
    QVERIFY2(m_model->getPageIncommingConnections(d).isEmpty(), "Page has incomming connection after connecting wrongly from EndPage");

    // Check Condition correct
    QVERIFY2(stk.execute(m_cmdFac->createConnection(c, a, GGConnectionSlot::TrueConnection)), "Cannot set True Connection");
    QVERIFY2(stk.execute(m_cmdFac->createConnection(c, e, GGConnectionSlot::FalseConnection)), "Cannot set False Connection");
    cn = static_cast<GGCreateConnectionCmd *> (stk.getUndoCommands()[1])->createdConnection();
    QVERIFY2(cn->source() == c && cn->destination() == a, "Src/Dest wrong for True page connection");
    QVERIFY2(c->trueConnection() == cn, "True connection not correct");
    QVERIFY2(m_model->getPageIncommingConnections(a).value(0) == cn, "Incomming not correct after setting True Connection");
    cn = static_cast<GGCreateConnectionCmd *> (stk.getUndoCommands()[0])->createdConnection();
    QVERIFY2(cn->source() == c && cn->destination() == e, "Src/Dest wrong for False page connection");
    QVERIFY2(c->falseConnection() == cn, "False connection not correct");
    QVERIFY2(m_model->getPageIncommingConnections(e).value(0) == cn, "Incomming not correct after setting False Connection");
    stk.undo();
    QVERIFY2(c->falseConnection() == 0, "False connection not unset with undo");
    QVERIFY2(m_model->getPageIncommingConnections(e).isEmpty(), "Incomming not unset correct after undo False Connection");
    stk.redo();
    QVERIFY2(c->falseConnection() == cn, "False connection not correct after redo");
    QVERIFY2(m_model->getPageIncommingConnections(e).value(0) == cn, "Incomming not correct after redo setting False Connection");

    // Try reset FALSE. This should not work with CreateConnection Command!
    int oldDinCount = m_model->getPageIncommingConnections(d).size();
    QVERIFY2(!stk.execute(m_cmdFac->createConnection(c, d, GGConnectionSlot::FalseConnection)), "Create connection on already connected slot succeeded");
    QVERIFY2(cn->source() == c && cn->destination() == e, "Src/Dest wrong for True page connection after re-setting");
    QVERIFY2(c->falseConnection() == cn, "False connection changed");
    QVERIFY2(m_model->getPageIncommingConnections(e).value(0) == cn, "Incomming reset after failing re-setting False connection on old page");
    QVERIFY2(m_model->getPageIncommingConnections(d).size() == oldDinCount, "Incomming reset after failing re-setting False connection on old page");
}
