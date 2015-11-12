#include "tst_ggeditor_modelcommandtest.h"

#include <command/ggcommandgroup.h>
#include <command/ggcommandstack.h>
#include <command/ggeditcommandfactory.h>
#include <command/ggmodelpagecommands.h>
#include <command/ggmodelconnectioncommands.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include "modelsignalchecker.h"

GGEditor_ModelCommandTest::GGEditor_ModelCommandTest(QObject *parent) : QObject(parent)
{

}

void GGEditor_ModelCommandTest::init()
{
    m_model = new GGEditModel(new GGSimpleFactory);
    m_fac = new GGEditCommandFactory(m_model);
    //m_stk = new GGCommandStack;
    m_sc = new ModelSignalChecker(m_model);
}

void GGEditor_ModelCommandTest::cleanup()
{
    delete m_sc;
    //delete m_stk;
    delete m_fac;
    delete m_model;
}

void GGEditor_ModelCommandTest::testCreatePage()
{
    GGCreatePageCmd *cmd = m_fac->createStartPage();

    QVERIFY2(cmd->execute(), qPrintable("Error executing CreatePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Create Start Page Cmd", 1, 0, 0, 0, 0);
    QVERIFY2(cmd->createdPage() != NULL, "CreatePage command has no created page");
    QVERIFY2(cmd->createdPage()->model() == m_model, "Created page is not associated with the model");
    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == cmd->createdPage(), "Created Page is not same as retrieved from model");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing CreatePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Undo Create Start Page", 0, 1, 0, 0, 0);
    QVERIFY2(cmd->createdPage()->model() == NULL, "Created page is still associated with model after undo");
    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == NULL, "Created Page is in model after undo");

    QVERIFY2(cmd->redo(), qPrintable("Error redoing CreatePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Redo Create Start Page Cmd", 1, 0, 0, 0, 0);
    QVERIFY2(cmd->createdPage()->model() == m_model, "Created page is not associated with the model after redo");
    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == cmd->createdPage(), "Created Page is not same as retrieved from model after redo");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice CreatePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Second Undo Create Start Page", 0, 1, 0, 0, 0);
    QVERIFY2(cmd->createdPage()->model() == NULL, "Created page is still associated with model after second undo");
    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == NULL, "Created Page is in model after second undo");

    VERIFYSIGNULL(m_sc, "After CreatePage");

    delete cmd;
}

void GGEditor_ModelCommandTest::testDeletePage()
{
    GGCreatePageCmd *cr = m_fac->createStartPage();
    cr->execute();
    GGPage *p = cr->createdPage();
    delete cr;
    m_sc->reset();

    GGDeletePageCmd *cmd = m_fac->deletePage(p);

    QVERIFY2(p == cmd->deletedPage(), "Deleted page is not the page to delete");

    QVERIFY2(cmd->execute(), qPrintable("Error executing DeletePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Delete Page", 0, 1, 0 ,0, 0);
    QVERIFY2(p->model() == NULL, "Page is still associated with model after delete");
    QVERIFY2(m_model->getPage(p->id()) == NULL, "Page is still in model after delete");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing DeletePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Undo Delete Page", 1, 0, 0 ,0, 0);
    QVERIFY2(cmd->deletedPage()->model() == m_model, "Deleted page is not associated with the model after undo");
    QVERIFY2(m_model->getPage(cmd->deletedPage()->id()) == cmd->deletedPage(), "Deleted Page is not same as retrieved from model after undo");

    QVERIFY2(cmd->redo(), qPrintable("Error redoing DeletePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Redo Delete Page", 0, 1, 0 ,0, 0);
    QVERIFY2(p->model() == NULL, "Page is still associated with model after redo");
    QVERIFY2(m_model->getPage(p->id()) == NULL, "Page is still in model after redo");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice DeletePage command: " + cmd->error()));
    VERIFYSIG(m_sc, "Second Undo Page", 1, 0, 0 ,0, 0);
    QVERIFY2(cmd->deletedPage()->model() == m_model, "Deleted page is not associated with the model after second undo");
    QVERIFY2(m_model->getPage(cmd->deletedPage()->id()) == cmd->deletedPage(), "Deleted Page is not same as retrieved from model after second undo");

    delete cmd;
}

void GGEditor_ModelCommandTest::testCreateConnection()
{
    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createStartPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createEndPage());

    GGPage *s = m_model->getPages()[0];
    GGPage *e = m_model->getPages()[1];
    m_sc->reset();

    GGCreateConnectionCmd *cmd = m_fac->createConnection(s, e, GGConnectionSlot::StartConnection);
    QVERIFY2(cmd->execute(), qPrintable("Error executing CreateConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Create Connection", 0, 0, 1, 0, 1);

    QVERIFY2(cmd->createdConnection() != NULL, "CreateConnection command has no created connection");
    QVERIFY2(cmd->createdConnection()->model() == m_model, "Created connection is not associated with the model");
    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == cmd->createdConnection(), "Created Connection is not same as retrieved from model");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing CreateConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Undo Create Connection", 0, 0, 0, 1, 1);
    QVERIFY2(cmd->createdConnection()->model() == NULL, "Created connection is still associated with model after undo");
    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == NULL, "Created connection is in model after undo");

    QVERIFY2(cmd->redo(), qPrintable("Error redoing CreateConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Redo Create Connection", 0, 0, 1, 0, 1);
    QVERIFY2(cmd->createdConnection()->model() == m_model, "Created connection is not associated with the model after redo");
    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == cmd->createdConnection(), "Created connection is not same as retrieved from model after redo");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice CreateConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Second Undo Create Connection", 0, 0, 0, 1, 1);
    QVERIFY2(cmd->createdConnection()->model() == NULL, "Created connection is still associated with model after second undo");
    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == NULL, "Created connection is in model after second undo");

    delete cmd;
}

void GGEditor_ModelCommandTest::testDeleteConnection()
{
    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createStartPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createEndPage());
    GGPage *s = m_model->getPages()[0];
    GGPage *e = m_model->getPages()[1];

    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createConnection(s, e, GGConnectionSlot::StartConnection));
    GGConnection *c = m_model->getConnections()[0];
    m_sc->reset();

    GGDeleteConnectionCmd *cmd = m_fac->deleteConnection(c);

    QVERIFY2(c == cmd->deletedConnection(), "Deleted connection is not the connection to delete");

    QVERIFY2(cmd->execute(), qPrintable("Error executing DeleteConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Delete Connectin", 0, 0, 0, 1, 1);
    QVERIFY2(c->model() == NULL, "Connection is still associated with model after delete");
    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after delete");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing DeleteConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Undo Delete Connectin", 0, 0, 1, 0, 1);
    QVERIFY2(cmd->deletedConnection()->model() == m_model, "Deleted connection is not associated with the model after undo");
    QVERIFY2(m_model->getConnection(cmd->deletedConnection()->id()) == cmd->deletedConnection(), "Deleted connection is not same as retrieved from model after undo");

    QVERIFY2(cmd->redo(), qPrintable("Error redoing DeleteConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Redo Delete Connectin", 0, 0, 0, 1, 1);
    QVERIFY2(c->model() == NULL, "Connection is still associated with model after redo");
    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after redo");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice DeleteConnection command: " + cmd->error()));
    VERIFYSIG(m_sc, "Second Undo Delete Connectin", 0, 0, 1, 0, 1);
    QVERIFY2(cmd->deletedConnection()->model() == m_model, "Deleted connection is not associated with the model after second undo");
    QVERIFY2(m_model->getConnection(cmd->deletedConnection()->id()) == cmd->deletedConnection(), "Deleted connection is not same as retrieved from model after second undo");
    delete cmd;
}

void GGEditor_ModelCommandTest::testDeletePageWithConnection()
{
    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createStartPage());
    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createEndPage());
    GGStartPage *p1 = ggpage_cast<GGStartPage*> (m_model->getPages()[0]);
    GGPage *p2 = m_model->getPages()[1];

    GGAbstractModelCommandFactory::oneShotCommand(m_fac->createConnection(p1, p2, GGConnectionSlot::StartConnection));
    GGConnection *c = m_model->getConnections()[0];

    m_sc->reset();

    GGDeletePageCmd *cmd = m_fac->deletePage(p1);
    QVERIFY2(cmd->execute(), "Error deleting src");
    VERIFYSIG(m_sc, "Delete Page with connnection", 0, 1, 0, 1, 0);
    QVERIFY2(c->model() == NULL, "Connection still associated with model after delete src");
    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after delete src");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after delete src");
    QVERIFY2(m_model->getPageIncommingConnections(p2).isEmpty(), "Dest still has incomming connections after delete src");

    QVERIFY2(cmd->undo(), "Error undoing delete of src");
    VERIFYSIG(m_sc, "Undo Delete Page with connnection", 1, 0, 1, 0, 1);
    QVERIFY2(c->model() == m_model, "Connection not associated with model after undo delete src");
    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after undo delete src");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after undo delete src");
    QVERIFY2(m_model->getPageIncommingConnections(p2).size() == 1, "Dest has no incomming connections after undo delete src");

    QVERIFY2(cmd->redo(), "Error redoing delete of src");
    VERIFYSIG(m_sc, "Redo Delete Page with connnection", 0, 1, 0, 1, 0);
    QVERIFY2(c->model() == NULL, "Connection still associated with model after redo delete src");
    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after redo delete src");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after redo delete src");
    QVERIFY2(m_model->getPageIncommingConnections(p2).isEmpty(), "Dest still has incomming connections after redo delete src");

    QVERIFY2(cmd->undo(), "Error undoing twice delete of src");
    VERIFYSIG(m_sc, "Second Undo Delete Page with connnection", 1, 0, 1, 0, 1);
    QVERIFY2(c->model() == m_model, "Connection not associated with model after second undo delete src");
    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after second undo delete src");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after second undo delete src");
    QVERIFY2(m_model->getPageIncommingConnections(p2).size() == 1, "Dest has no incomming connections after second undo delete src");
    delete cmd;

    cmd = m_fac->deletePage(p2);
    QVERIFY2(cmd->execute(), "Error deleting dest");
    VERIFYSIG(m_sc, "Delete Page with connnection", 0, 1, 0, 1, 1);
    QVERIFY2(c->model() == NULL, "Connection still associated with model after delete dest");
    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after delete dest");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after delete dest");

    QVERIFY2(cmd->undo(), "Error undoing delete of dest");
    VERIFYSIG(m_sc, "Undo Delete Page with connnection", 1, 0, 1, 0, 1);
    QVERIFY2(c->model() == m_model, "Connection not associated with model after undo delete dest");
    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after undo delete dest");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after undo delete dest");

    QVERIFY2(cmd->redo(), "Error redoing delete of dest");
    VERIFYSIG(m_sc, "Redo Delete Page with connnection", 0, 1, 0, 1, 1);
    QVERIFY2(c->model() == NULL, "Connection still associated with model after redo delete dest");
    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after redo delete dest");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after redo delete dest");

    QVERIFY2(cmd->undo(), "Error undoing twice delete of dest");
    VERIFYSIG(m_sc, "Second Undo Delete Page with connnection", 1, 0, 1, 0, 1);
    QVERIFY2(c->model() == m_model, "Connection not associated with model after second undo delete dest");
    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after second undo delete dest");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after second undo delete dest");
    delete cmd;
}
