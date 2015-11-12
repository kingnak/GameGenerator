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
#include "modelsignalchecker.h"

void GGEditor_ConnectionSlotTest::init()
{
    m_model = new GGEditModel(new GGSimpleFactory);
    m_cmdFac = new GGEditCommandFactory(m_model);
    m_stack = new GGCommandStack;
    m_sc = new ModelSignalChecker(m_model);

    GGAbstractModelCommandFactory::oneShotCommand(m_cmdFac->createStartPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_cmdFac->createEndPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_cmdFac->createConditionPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_cmdFac->createActionPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_cmdFac->createDecisionPage());

    m_s = ggpage_cast<GGStartPage*> (m_model->getPages()[0]);
    m_e = ggpage_cast<GGEndPage*> (m_model->getPages()[1]);
    m_c = ggpage_cast<GGConditionPage*> (m_model->getPages()[2]);
    m_a = ggpage_cast<GGActionPage*> (m_model->getPages()[3]);
    m_d = ggpage_cast<GGDecisionPage*> (m_model->getPages()[4]);

    m_sc->verify("init", 5, 0, 0, 0, 0);
}

void GGEditor_ConnectionSlotTest::cleanup()
{
    delete m_sc;
    delete m_stack;
    delete m_cmdFac;
    delete m_model;
}

void GGEditor_ConnectionSlotTest::testConnectStart()
{
    // Check correct start connection
    QVERIFY(m_stack->execute(m_cmdFac->createConnection(m_s, m_e, GGConnectionSlot::StartConnection)));
    VERIFYSIG(m_sc, "Create connection", 0, 0, 1, 0, 1);

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
    VERIFYSIG(m_sc, "Create Connection", 0, 0, 1, 1, 2);

    // Check create conn with existing conn
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::StartConnection)), "Can create connection with existing connection in slot");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).isEmpty(), "Wrong connection resulted in an incomming connection");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).value(0) == cn, "Old connection lost as incommong after wrong connect");
    QVERIFY2(m_s->startConnection() == cn, "Outgoing connection lost after wrong connect");
    VERIFYSIGNULL(m_sc, "Create Connection when one already existed");

    // Check exchange
    QVERIFY2(m_stack->execute(m_cmdFac->exchangeConnection(m_s, m_d, GGConnectionSlot::StartConnection)), "Cannot exchange connection");
    GGConnection *cn2 = static_cast<GGExchangeConnectionCmd*>(m_stack->undoCommand())->newConnection();
    GGConnection *cn3 = static_cast<GGExchangeConnectionCmd*>(m_stack->undoCommand())->oldConnection();
    QVERIFY2(cn3 == cn, "Old connection after exchange is not previous connection");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).isEmpty(), "Old connection still there as incommong after exchange");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).value(0) == cn2, "Exchange did not set incomming connection");
    QVERIFY2(m_s->startConnection() == cn2, "Outgoing connection set wrong after exchange");
    VERIFYSIG(m_sc, "Exchange Connection", 0, 0, 1, 1, 2);

    // Check undo exchange
    QVERIFY(m_stack->undo());
    QVERIFY2(m_s->startConnection() == cn, "Connection not same after undoing Exchange");
    QVERIFY2(m_model->getPageIncommingConnections(m_d).isEmpty(), "New connection still there as incommong after undoing exchange");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).value(0) == cn, "Undoing Exchange did not reset incomming connection");
    VERIFYSIG(m_sc, "Undo Exchange Connection", 0, 0, 1, 1, 2);

    // Check wrong start connection
    m_stack->undo();
    VERIFYSIG(m_sc, "Undo Create connection", 0, 0, 0, 1, 1);
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_e, GGConnectionSlot::ActionConnection)), "Can connect start page with wrong slot");
    QVERIFY2(m_s->startConnection() == 0, "Connection set in StartPage after using wrong slot");
    QVERIFY2(m_model->getPageIncommingConnections(m_e).size() == 0, "Connection incomming for start page after using wrong slot");
    VERIFYSIGNULL(m_sc, "Create Wrong Connection");

    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::ActionConnection)), "Can connect Start with Action");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::TrueConnection)), "Can connect Start with True");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot::FalseConnection)), "Can connect Start with False");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect Start with Decision");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_s, m_d, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Can connect Start with Decision");
    VERIFYSIGNULL(m_sc, "Create Wrong Connection");
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
    VERIFYSIGNULL(m_sc, "Create Wrong Connection");
}

void GGEditor_ConnectionSlotTest::testConnectCondition()
{
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot::ActionConnection)), "Can connect Condition with Action");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot::StartConnection)), "Can connect Condition with Start");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect Condition with Decision");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Can connect Condition with Decision");
    VERIFYSIGNULL(m_sc, "Create Wrong Connection");

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_c, m_e, GGConnectionSlot::TrueConnection)), "Cannot set True connection");
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_c, m_d, GGConnectionSlot::FalseConnection)), "Cannot set False connection");
    GGConnection *c2 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY(m_c->trueConnection() == c1);
    QVERIFY(m_c->falseConnection() == c2);
    VERIFYSIG(m_sc, "Connect Condition", 0, 0, 2, 0, 2);
}

void GGEditor_ConnectionSlotTest::testActionConnection()
{
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot::TrueConnection)), "Can connect Action with True");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot::FalseConnection)), "Can connect Action with False");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot::StartConnection)), "Can connect Action with Start");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_a, m_d, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can connect Action with Decision");
    VERIFYSIGNULL(m_sc, "Create Wrong Connection");

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_a, m_e, GGConnectionSlot::ActionConnection)), "Cannot set Action connection");
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY(c1 == m_a->actionLink().connection());
    VERIFYSIG(m_sc, "Connect Action", 0, 0, 1, 0, 1);
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
    mcp->addMappedLink(GGMappedLink::rectangle(QRect()));
    VERIFYSIG(m_sc, "Add Map Links", 0, 0, 0, 0, 2);

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(mcp, m_e, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Cannot set MappedConnection");
    VERIFYSIG(m_sc, "Connect Map", 0, 0, 1, 0, 1);
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Link's connection is wrong after setting MappedConnection");
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == 0, "Wrong link set in MappedConnection");
    QVERIFY(m_stack->undo());
    VERIFYSIG(m_sc, "Undo Connect Map", 0, 0, 0, 1, 1);
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == 0, "Link's connection is still set after undoing setting MappedConnection");
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == 0, "Wrong link set in MappedConnection");
    QVERIFY(m_stack->redo());
    VERIFYSIG(m_sc, "Redo Connect Map", 0, 0, 1, 0, 1);
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Link's connection is wrong after redoing setting MappedConnection");
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == 0, "Wrong link set in MappedConnection");

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(mcp, m_c, GGConnectionSlot(GGConnectionSlot::MappedConnection, 1))), "Cannot set MappedConnection");
    VERIFYSIG(m_sc, "Connect Map", 0, 0, 1, 0, 1);
    GGConnection *c2 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == c2, "Link's connection is wrong after setting MappedConnection");
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Wrong link set in MappedConnection");
    QVERIFY(m_stack->undo());
    VERIFYSIG(m_sc, "Undo Connect Map", 0, 0, 0, 1, 1);
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == 0, "Link's connection is still set after undoing setting MappedConnection");
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Wrong link set in MappedConnection");
    QVERIFY(m_stack->redo());
    VERIFYSIG(m_sc, "Redo Connect Map", 0, 0, 1, 0, 1);
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == c2, "Link's connection is wrong after redoing setting MappedConnection");
    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == c1, "Wrong link set in MappedConnection");

    QVERIFY2(mcp->getLinkMap().value(0).link().connection() == mcp->getMappedConnections().value(0), "Link's connection is not same as mapped connection");
    QVERIFY2(mcp->getLinkMap().value(1).link().connection() == mcp->getMappedConnections().value(1), "Link's connection is not same as mapped connection");
    VERIFYSIGNULL(m_sc, "Checking Map");
}

void GGEditor_ConnectionSlotTest::testDecisionConnection()
{
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_d, m_e, GGConnectionSlot::TrueConnection)), "Can connect Decision with True");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_d, m_e, GGConnectionSlot::FalseConnection)), "Can connect Decision with False");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_d, m_e, GGConnectionSlot::StartConnection)), "Can connect Decision with Start");
    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_d, m_e, GGConnectionSlot::ActionConnection)), "Can connect Decision with Action");
    VERIFYSIGNULL(m_sc, "Create Wrong Connection");

    QVERIFY2(!m_stack->execute(m_cmdFac->createConnection(m_d, m_e, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Can set DecisionConnection without link");
    m_d->addDecisionLink(GGLink());
    m_d->addDecisionLink(GGLink());
    VERIFYSIG(m_sc, "Add Decisions", 0, 0, 0, 0, 2);
    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_d, m_e, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Cannot set DecisionConnection");
    VERIFYSIG(m_sc, "Connect Decision", 0, 0, 1, 0, 1);
    GGConnection *c1 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(m_d->getDecisionConnections().value(0) == c1, "Link's connection is wrong after setting DecisionConnection");
    QVERIFY2(m_d->getDecisionConnections().value(1) == 0, "Wrong link set in DecisionConnection");
    QVERIFY(m_stack->undo());
    VERIFYSIG(m_sc, "Undo Connect Decision", 0, 0, 0, 1, 1);
    QVERIFY2(m_d->getDecisionConnections().value(0) == 0, "Link's connection is still set after undoing setting DecisionConnection");
    QVERIFY2(m_d->getDecisionConnections().value(1) == 0, "Wrong link set in DecisionConnection");
    QVERIFY(m_stack->redo());
    VERIFYSIG(m_sc, "Redo Connect Decision", 0, 0, 1, 0, 1);
    QVERIFY2(m_d->getDecisionConnections().value(0) == c1, "Link's connection is wrong after redoing setting DecisionConnection");
    QVERIFY2(m_d->getDecisionConnections().value(1) == 0, "Wrong link set in DecisionConnection");

    QVERIFY2(m_stack->execute(m_cmdFac->createConnection(m_d, m_c, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 1))), "Cannot set DecisionConnection");
    VERIFYSIG(m_sc, "Connect Decision", 0, 0, 1, 0, 1);
    GGConnection *c2 = static_cast<GGCreateConnectionCmd*> (m_stack->undoCommand())->createdConnection();
    QVERIFY2(m_d->getDecisionConnections().value(1) == c2, "Link's connection is wrong after setting DecisionConnection");
    QVERIFY2(m_d->getDecisionConnections().value(0) == c1, "Wrong link set in DecisionConnection");
    QVERIFY(m_stack->undo());
    VERIFYSIG(m_sc, "Undo Connect Decision", 0, 0, 0, 1, 1);
    QVERIFY2(m_d->getDecisionConnections().value(1) == 0, "Link's connection is still set after undoing setting DecisionConnection");
    QVERIFY2(m_d->getDecisionConnections().value(0) == c1, "Wrong link set in DecisionConnection");
    QVERIFY(m_stack->redo());
    VERIFYSIG(m_sc, "Redo Connect Decision", 0, 0, 1, 0, 1);
    QVERIFY2(m_d->getDecisionConnections().value(1) == c2, "Link's connection is wrong after redoing setting DecisionConnection");
    QVERIFY2(m_d->getDecisionConnections().value(0) == c1, "Wrong link set in DecisionConnection");

    QVERIFY2(m_d->getDecisionLinks().value(0).connection() == m_d->getDecisionConnections().value(0), "Link's connection is not same as decision connection");
    QVERIFY2(m_d->getDecisionLinks().value(1).connection() == m_d->getDecisionConnections().value(1), "Link's connection is not same as decision connection");
    VERIFYSIGNULL(m_sc, "Checking Decisions");
}
