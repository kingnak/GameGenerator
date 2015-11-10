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
    m_stack = new GGCommandStack;

    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createStartPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createEndPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createConditionPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createActionPage());
    GGAbstractCommandFactory::oneShotCommand(m_cmdFac->createDecisionPage());

    m_s = ggpage_cast<GGStartPage*> (m_model->getPages()[0]);
    m_e = ggpage_cast<GGEndPage*> (m_model->getPages()[1]);
    m_c = ggpage_cast<GGConditionPage*> (m_model->getPages()[2]);
    m_a = ggpage_cast<GGActionPage*> (m_model->getPages()[3]);
    m_d = ggpage_cast<GGDecisionPage*> (m_model->getPages()[4]);
}

void GGEditor_ConnectionSlotTest::cleanup()
{
    delete m_stack;
    delete m_cmdFac;
    delete m_model;
}

void GGEditor_ConnectionSlotTest::testConnectStart()
{
    // Check correct start connection
    QVERIFY(m_stack->execute(m_cmdFac->createConnection(m_s, m_e, GGConnectionSlot::StartConnection)));
    GGConnection *cn = static_cast<GGCreateConnectionCmd *> (m_stack->getAllCommands()[0])->createdConnection();
    QVERIFY2(m_s->startConnection() == cn, "Connection not set in StartPage");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).value(0) == cn, "Connection not incomming for start page");
    QVERIFY2(cn->source() == m_s && cn->destination() == m_e, "Src/Dest wrong for start page connection");
    QVERIFY(m_stack->undo());
    QVERIFY2(m_s->startConnection() == 0, "Connection not unset in StartPage");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).size() == 0, "Connection still incomming for start page");
    QVERIFY(m_stack->redo());
    QVERIFY2(m_s->startConnection() == cn, "Connection not set in StartPage");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).value(0) == cn, "Connection not incomming for start page");

    // Check create conn with existing conn
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::StartConnection)), "Can create connection with existing connection in slot");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).isEmpty(), "Wrong connection resulted in an incomming connection");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).value(0) == cn, "Old connection lost as incommong after wrong connect");
    QVERIFY2(m_s->startConnection() == cn, "Outgoing connection lost after wrong connect");

    // Check exchange
    QVERIFY2(m_stack->execute(m_cmdFac->exchangeConnection(m_s, m_d, GGConnectionSlot::StartConnection)), "Cannot exchange connection");
    GGConnection *cn2 = static_cast<GGExchangeConnectionCmd*>(m_stack->undoCommand())->newConnection();
    GGConnection *cn3 = static_cast<GGExchangeConnectionCmd*>(m_stack->undoCommand())->oldConnection();
    QVERIFY2(cn3 == cn, "Old connection after exchange is not previous connection");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).isEmpty(), "Old connection still there as incommong after exchange");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).value(0) == cn2, "Exchange did not set incomming connection");
    QVERIFY2(m_s->startConnection() == cn2, "Outgoing connection set wrong after exchange");

    // Check undo exchange
    QVERIFY(m_stack->undo());
    QVERIFY2(m_s->startConnection() == cn, "Connection not same after undoing Exchange");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).isEmpty(), "New connection still there as incommong after undoing exchange");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).value(0) == cn, "Undoing Exchange did not reset incomming connection");

    // Check wrong start connection
    m_stack->undo();
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_e, GGConnectionSlot::ActionConnection)), "Can connect start page with wrong slot");
    QVERIFY2(m_s->startConnection() == 0, "Connection set in StartPage after using wrong slot");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).size() == 0, "Connection incomming for start page after using wrong slot");

    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::ActionConnection)), "Can connect Start with Action");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::TrueConnection)), "Can connect Start with True");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::FalseConnection)), "Can connect Start with False");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect Start with Decision");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Can connect Start with Decision");
}

void GGEditor_ConnectionSlotTest::testConnectEnd()
{
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_e, m_d, GGConnectionSlot::ActionConnection)), "Can connect end with Action");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_e, m_d, GGConnectionSlot::TrueConnection)), "Can connect end with True");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_e, m_d, GGConnectionSlot::FalseConnection)), "Can connect end with False");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_e, m_d, GGConnectionSlot::StartConnection)), "Can connect end with Start");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_e, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect end with Decision");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_e, m_d, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Can connect end with Decision");
    QVERIFY2(m_e->getConnections().isEmpty(), "End has outgoing connections");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).isEmpty(), "Destination has incomming when connecting from End");
}

void GGEditor_ConnectionSlotTest::testConnectCondition()
{
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot::ActionConnection)), "Can connect Condition with Action");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot::StartConnection)), "Can connect Condition with Start");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect Condition with Decision");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Can connect Condition with Decision");

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_c, m_e, GGConnectionSlot::TrueConnection)), "Cannot set True connection");
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot::FalseConnection)), "Cannot set False connection");
    GGConnection *c2 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY(m_c->trueConnection() == c1);
    QVERIFY(m_c->falseConnection() == c2);
}

void GGEditor_ConnectionSlotTest::testActionConnection()
{
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot::TrueConnection)), "Can connect Action with True");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot::FalseConnection)), "Can connect Action with False");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot::StartConnection)), "Can connect Action with Start");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect Action with Decision");

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_a, m_e, GGConnectionSlot::ActionConnection)), "Cannot set Action connection");
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY(c1 == m_a->actionLink().connection());
}

void GGEditor_ConnectionSlotTest::testMappedConnection_data()
{
    QTest::addColumn<bool>("type");
    QTest::newRow("Action") << true;
    QTest::newRow("Decision") << false;
}

void GGEditor_ConnectionSlotTest::testMappedConnection()
{
    GGMappedContentPage *mcp;
    QFETCH(bool, type);
    if (type) {
        mcp = m_a;
    } else {
        mcp = m_d;
    }

    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(mcp, m_e, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Can set MappedConnection without link");
    mcp->addMappedLink(GGMappedLink::rectangle(QRect()));
    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(mcp, m_e, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Cannot set MappedConnection");
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Link's connection is wrong after setting MappedConnection");
    QVERIFY(m_stack->undo());
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == 0, "Link's connection is still set after undoing setting MappedConnection");
    QVERIFY(m_stack->redo());
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Link's connection is wrong after redoing setting MappedConnection");
}

/*
void GGEditor_ConnectionSlotTest::t(){

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

    // Try reset FALSE. This must work with ExchangeConnection Command
    QVERIFY2(stk.execute(m_cmdFac->exchangeConnection(c, d, GGConnectionSlot::FalseConnection)), "Cannot exchange connection");
    QVERIFY2(c->falseConnection() != cn, "Old False Connection still set");
    QVERIFY2(m_model->getPageIncommingConnections(e).size() == 0, "Old destination still has incomming");
    QVERIFY2(static_cast<GGExchangeConnectionCmd*>(stk.undoCommand())->oldConnection() == cn, "Old connection is not the old False connection");



}
*/
