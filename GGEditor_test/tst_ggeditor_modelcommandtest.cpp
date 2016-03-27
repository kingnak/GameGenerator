//#include "tst_ggeditor_modelcommandtest.h"

//#include <command/ggcommandgroup.h>
//#include <command/ggcommandstack.h>
//#include <command/ggeditcommandfactory.h>
//#include <command/ggmodelpagecommands.h>
//#include <command/ggmodelconnectioncommands.h>
//#include <model/ggeditmodel.h>
//#include <model/ggsimplefactory.h>
//#include <model/ggpage.h>
//#include <model/ggconnection.h>
//#include <model/ggcontentelement.h>
//#include "testsignalchecker.h"

//GGEditor_ModelCommandTest::GGEditor_ModelCommandTest(QObject *parent) : QObject(parent)
//{

//}

//void GGEditor_ModelCommandTest::init()
//{
//    m_model = new GGEditModel(new GGSimpleFactory, new GGFileSystemResolver);
//    m_fac = new GGEditCommandFactory(m_model);
//    m_stk = new GGCommandStack;
//    m_sc = new ModelSignalChecker(m_model);
//}

//void GGEditor_ModelCommandTest::cleanup()
//{
//    delete m_sc;
//    delete m_stk;
//    delete m_fac;
//    delete m_model;
//}

//void GGEditor_ModelCommandTest::testCreatePage()
//{
//    GGCreatePageCmd *cmd = m_fac->createStartPage();

//    QVERIFY2(cmd->execute(), qPrintable("Error executing CreatePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Create Start Page Cmd", 1, 0, 0, 0, 0);
//    QVERIFY2(cmd->createdPage() != NULL, "CreatePage command has no created page");
//    QVERIFY2(cmd->createdPage()->model() == m_model, "Created page is not associated with the model");
//    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == cmd->createdPage(), "Created Page is not same as retrieved from model");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing CreatePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Undo Create Start Page", 0, 1, 0, 0, 0);
//    QVERIFY2(cmd->createdPage()->model() == NULL, "Created page is still associated with model after undo");
//    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == NULL, "Created Page is in model after undo");

//    QVERIFY2(cmd->redo(), qPrintable("Error redoing CreatePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Redo Create Start Page Cmd", 1, 0, 0, 0, 0);
//    QVERIFY2(cmd->createdPage()->model() == m_model, "Created page is not associated with the model after redo");
//    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == cmd->createdPage(), "Created Page is not same as retrieved from model after redo");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice CreatePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Second Undo Create Start Page", 0, 1, 0, 0, 0);
//    QVERIFY2(cmd->createdPage()->model() == NULL, "Created page is still associated with model after second undo");
//    QVERIFY2(m_model->getPage(cmd->createdPage()->id()) == NULL, "Created Page is in model after second undo");

//    VERIFYSIGNULL(m_sc, "After CreatePage");

//    delete cmd;
//}

//void GGEditor_ModelCommandTest::testDeletePage()
//{
//    GGCreatePageCmd *cr = m_fac->createStartPage();
//    cr->execute();
//    GGPage *p = cr->createdPage();
//    delete cr;
//    m_sc->reset();

//    GGDeletePageCmd *cmd = m_fac->deletePage(p);

//    QVERIFY2(p == cmd->deletedPage(), "Deleted page is not the page to delete");

//    QVERIFY2(cmd->execute(), qPrintable("Error executing DeletePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Delete Page", 0, 1, 0 ,0, 0);
//    QVERIFY2(p->model() == NULL, "Page is still associated with model after delete");
//    QVERIFY2(m_model->getPage(p->id()) == NULL, "Page is still in model after delete");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing DeletePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Undo Delete Page", 1, 0, 0 ,0, 0);
//    QVERIFY2(cmd->deletedPage()->model() == m_model, "Deleted page is not associated with the model after undo");
//    QVERIFY2(m_model->getPage(cmd->deletedPage()->id()) == cmd->deletedPage(), "Deleted Page is not same as retrieved from model after undo");

//    QVERIFY2(cmd->redo(), qPrintable("Error redoing DeletePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Redo Delete Page", 0, 1, 0 ,0, 0);
//    QVERIFY2(p->model() == NULL, "Page is still associated with model after redo");
//    QVERIFY2(m_model->getPage(p->id()) == NULL, "Page is still in model after redo");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice DeletePage command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Second Undo Page", 1, 0, 0 ,0, 0);
//    QVERIFY2(cmd->deletedPage()->model() == m_model, "Deleted page is not associated with the model after second undo");
//    QVERIFY2(m_model->getPage(cmd->deletedPage()->id()) == cmd->deletedPage(), "Deleted Page is not same as retrieved from model after second undo");

//    delete cmd;
//}

//void GGEditor_ModelCommandTest::testCreateConnection()
//{
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createStartPage());
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createEndPage());

//    GGPage *s = m_model->getPages()[0];
//    GGPage *e = m_model->getPages()[1];
//    m_sc->reset();

//    GGCreateConnectionCmd *cmd = m_fac->createConnection(s, e, GGConnectionSlot::StartConnection);
//    QVERIFY2(cmd->execute(), qPrintable("Error executing CreateConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Create Connection", 0, 0, 1, 0, 1);

//    QVERIFY2(cmd->createdConnection() != NULL, "CreateConnection command has no created connection");
//    QVERIFY2(cmd->createdConnection()->model() == m_model, "Created connection is not associated with the model");
//    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == cmd->createdConnection(), "Created Connection is not same as retrieved from model");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing CreateConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Undo Create Connection", 0, 0, 0, 1, 1);
//    QVERIFY2(cmd->createdConnection()->model() == NULL, "Created connection is still associated with model after undo");
//    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == NULL, "Created connection is in model after undo");

//    QVERIFY2(cmd->redo(), qPrintable("Error redoing CreateConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Redo Create Connection", 0, 0, 1, 0, 1);
//    QVERIFY2(cmd->createdConnection()->model() == m_model, "Created connection is not associated with the model after redo");
//    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == cmd->createdConnection(), "Created connection is not same as retrieved from model after redo");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice CreateConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Second Undo Create Connection", 0, 0, 0, 1, 1);
//    QVERIFY2(cmd->createdConnection()->model() == NULL, "Created connection is still associated with model after second undo");
//    QVERIFY2(m_model->getConnection(cmd->createdConnection()->id()) == NULL, "Created connection is in model after second undo");

//    delete cmd;
//}

//void GGEditor_ModelCommandTest::testDeleteConnection()
//{
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createStartPage());
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createEndPage());
//    GGPage *s = m_model->getPages()[0];
//    GGPage *e = m_model->getPages()[1];

//    GGAbstractCommandFactory::oneShotCommand(m_fac->createConnection(s, e, GGConnectionSlot::StartConnection));
//    GGConnection *c = m_model->getConnections()[0];
//    m_sc->reset();

//    GGDeleteConnectionCmd *cmd = m_fac->deleteConnection(c);

//    QVERIFY2(c == cmd->deletedConnection(), "Deleted connection is not the connection to delete");

//    QVERIFY2(cmd->execute(), qPrintable("Error executing DeleteConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Delete Connectin", 0, 0, 0, 1, 1);
//    QVERIFY2(c->model() == NULL, "Connection is still associated with model after delete");
//    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after delete");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing DeleteConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Undo Delete Connectin", 0, 0, 1, 0, 1);
//    QVERIFY2(cmd->deletedConnection()->model() == m_model, "Deleted connection is not associated with the model after undo");
//    QVERIFY2(m_model->getConnection(cmd->deletedConnection()->id()) == cmd->deletedConnection(), "Deleted connection is not same as retrieved from model after undo");

//    QVERIFY2(cmd->redo(), qPrintable("Error redoing DeleteConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Redo Delete Connectin", 0, 0, 0, 1, 1);
//    QVERIFY2(c->model() == NULL, "Connection is still associated with model after redo");
//    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after redo");

//    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice DeleteConnection command: " + cmd->error()));
//    VERIFYSIG(m_sc, "Second Undo Delete Connectin", 0, 0, 1, 0, 1);
//    QVERIFY2(cmd->deletedConnection()->model() == m_model, "Deleted connection is not associated with the model after second undo");
//    QVERIFY2(m_model->getConnection(cmd->deletedConnection()->id()) == cmd->deletedConnection(), "Deleted connection is not same as retrieved from model after second undo");
//    delete cmd;
//}

//void GGEditor_ModelCommandTest::testDeletePageWithConnection()
//{
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createStartPage());
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createEndPage());
//    GGStartPage *p1 = ggpage_cast<GGStartPage*> (m_model->getPages()[0]);
//    GGPage *p2 = m_model->getPages()[1];

//    GGAbstractCommandFactory::oneShotCommand(m_fac->createConnection(p1, p2, GGConnectionSlot::StartConnection));
//    GGConnection *c = m_model->getConnections()[0];

//    m_sc->reset();

//    GGDeletePageCmd *cmd = m_fac->deletePage(p1);
//    QVERIFY2(cmd->execute(), "Error deleting src");
//    VERIFYSIG(m_sc, "Delete Page with connnection", 0, 1, 0, 1, 0);
//    QVERIFY2(c->model() == NULL, "Connection still associated with model after delete src");
//    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after delete src");
//    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after delete src");
//    QVERIFY2(m_model->getPageIncommingConnections(p2).isEmpty(), "Dest still has incomming connections after delete src");

//    QVERIFY2(cmd->undo(), "Error undoing delete of src");
//    VERIFYSIG(m_sc, "Undo Delete Page with connnection", 1, 0, 1, 0, 1);
//    QVERIFY2(c->model() == m_model, "Connection not associated with model after undo delete src");
//    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after undo delete src");
//    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after undo delete src");
//    QVERIFY2(m_model->getPageIncommingConnections(p2).size() == 1, "Dest has no incomming connections after undo delete src");

//    QVERIFY2(cmd->redo(), "Error redoing delete of src");
//    VERIFYSIG(m_sc, "Redo Delete Page with connnection", 0, 1, 0, 1, 0);
//    QVERIFY2(c->model() == NULL, "Connection still associated with model after redo delete src");
//    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after redo delete src");
//    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after redo delete src");
//    QVERIFY2(m_model->getPageIncommingConnections(p2).isEmpty(), "Dest still has incomming connections after redo delete src");

//    QVERIFY2(cmd->undo(), "Error undoing twice delete of src");
//    VERIFYSIG(m_sc, "Second Undo Delete Page with connnection", 1, 0, 1, 0, 1);
//    QVERIFY2(c->model() == m_model, "Connection not associated with model after second undo delete src");
//    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after second undo delete src");
//    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after second undo delete src");
//    QVERIFY2(m_model->getPageIncommingConnections(p2).size() == 1, "Dest has no incomming connections after second undo delete src");
//    delete cmd;

//    cmd = m_fac->deletePage(p2);
//    QVERIFY2(cmd->execute(), "Error deleting dest");
//    VERIFYSIG(m_sc, "Delete Page with connnection", 0, 1, 0, 1, 1);
//    QVERIFY2(c->model() == NULL, "Connection still associated with model after delete dest");
//    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after delete dest");
//    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after delete dest");

//    QVERIFY2(cmd->undo(), "Error undoing delete of dest");
//    VERIFYSIG(m_sc, "Undo Delete Page with connnection", 1, 0, 1, 0, 1);
//    QVERIFY2(c->model() == m_model, "Connection not associated with model after undo delete dest");
//    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after undo delete dest");
//    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after undo delete dest");

//    QVERIFY2(cmd->redo(), "Error redoing delete of dest");
//    VERIFYSIG(m_sc, "Redo Delete Page with connnection", 0, 1, 0, 1, 1);
//    QVERIFY2(c->model() == NULL, "Connection still associated with model after redo delete dest");
//    QVERIFY2(m_model->getConnection(c->id()) == NULL, "Connection is still in model after redo delete dest");
//    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after redo delete dest");

//    QVERIFY2(cmd->undo(), "Error undoing twice delete of dest");
//    VERIFYSIG(m_sc, "Second Undo Delete Page with connnection", 1, 0, 1, 0, 1);
//    QVERIFY2(c->model() == m_model, "Connection not associated with model after second undo delete dest");
//    QVERIFY2(m_model->getConnection(c->id()) == c, "Connection is still in model after second undo delete dest");
//    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after second undo delete dest");
//    delete cmd;
//}

//void GGEditor_ModelCommandTest::testSimpleSetterCommands()
//{
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createActionPage());
//    GGActionPage *p = ggpage_cast<GGActionPage *> (m_model->getPages()[0]);
//    m_sc->reset();

//    QVERIFY2(m_stk->execute(m_fac->setPageName(p, "name")), "Cannot set page name");
//    VERIFYSIG(m_sc, "Set Name", 0, 0, 0, 0, 1);
//    QCOMPARE(p->name(), QString("name"));
//    QVERIFY2(m_stk->undo(), "Cannot undo set page name");
//    VERIFYSIG(m_sc, "Undo Set Name", 0, 0, 0, 0, 1);
//    QCOMPARE(p->name(), QString());
//    QVERIFY2(m_stk->redo(), "Cannot undo set page name");
//    VERIFYSIG(m_sc, "Redo Set Name", 0, 0, 0, 0, 1);
//    QCOMPARE(p->name(), QString("name"));
//    QVERIFY2(m_stk->execute(m_fac->setPageName(p, "name")), "Cannot set page name");
//    VERIFYSIGNULL(m_sc, "Setting same name resulted in signal");

////    QVERIFY2(m_stk->execute(m_fac->setPageScene(p, "scene")), "Cannot set page scene");
////    VERIFYSIG(m_sc, "Set scene", 0, 0, 0, 0, 1);
////    QCOMPARE(p->sceneName(), QString("scene"));
////    QVERIFY2(m_stk->undo(), "Cannot undo set page scene");
////    VERIFYSIG(m_sc, "Undo Set scene", 0, 0, 0, 0, 1);
////    QCOMPARE(p->sceneName(), QString());
////    QVERIFY2(m_stk->redo(), "Cannot undo set page scene");
////    VERIFYSIG(m_sc, "Redo Set scene", 0, 0, 0, 0, 1);
////    QCOMPARE(p->sceneName(), QString("scene"));
////    QVERIFY2(m_stk->execute(m_fac->setPageScene(p, "scene")), "Cannot set page scene");
////    VERIFYSIGNULL(m_sc, "Setting same scene resulted in signal");

//    QVERIFY2(m_stk->execute(m_fac->setPageCaption(p, "caption")), "Cannot set page caption");
//    VERIFYSIG(m_sc, "Set caption", 0, 0, 0, 0, 1);
//    QCOMPARE(p->caption(), QString("caption"));
//    QVERIFY2(m_stk->undo(), "Cannot undo set page caption");
//    VERIFYSIG(m_sc, "Undo Set caption", 0, 0, 0, 0, 1);
//    QCOMPARE(p->caption(), QString());
//    QVERIFY2(m_stk->redo(), "Cannot undo set page caption");
//    VERIFYSIG(m_sc, "Redo Set caption", 0, 0, 0, 0, 1);
//    QCOMPARE(p->caption(), QString("caption"));
//    QVERIFY2(m_stk->execute(m_fac->setPageCaption(p, "caption")), "Cannot set page caption");
//    VERIFYSIGNULL(m_sc, "Setting same caption resulted in signal");

//    GGLink l;
//    l.setName("link");
//    QVERIFY2(m_stk->execute(m_fac->setActionLink(p, l)), "Cannot set action link");
//    VERIFYSIG(m_sc, "Set action link", 0, 0, 0, 0, 1);
//    QCOMPARE(p->actionLink().name(), QString("link"));
//    QVERIFY2(m_stk->undo(), "Cannot undo set page action link");
//    VERIFYSIG(m_sc, "Undo Set action link", 0, 0, 0, 0, 1);
//    QCOMPARE(p->actionLink().name(), QString());
//    QVERIFY2(m_stk->redo(), "Cannot undo set page action link");
//    VERIFYSIG(m_sc, "Redo Set action link", 0, 0, 0, 0, 1);
//    QCOMPARE(p->actionLink().name(), QString("link"));

//    m_stk->execute(m_fac->createEndPage());
//    GGPage *e = static_cast<GGCreatePageCmd*> (m_stk->undoCommand())->createdPage();
//    m_stk->execute(m_fac->createConnection(p, e, GGConnectionSlot::ActionConnection));
//    GGConnection *c = static_cast<GGCreateConnectionCmd*> (m_stk->undoCommand())->createdConnection();
//    QVERIFY2(p->actionLink().connection() == c, "Connection not set in link");

//    m_sc->reset();
//    QVERIFY2(m_stk->execute(m_fac->setActionLink(p, l)), "Cannot set action link when connection is there");
//    VERIFYSIG(m_sc, "Set action link with existing connection", 0, 0, 0, 0, 1);
//    QVERIFY2(p->actionLink().connection() == c, "Connection not set in new link");
//    m_stk->undo();
//    VERIFYSIG(m_sc, "Undo Set action link with existing connection", 0, 0, 0, 0, 1);
//    QVERIFY2(p->actionLink().connection() == c, "Connection not set in new link after undo");
//    m_stk->redo();
//    VERIFYSIG(m_sc, "Redo Set action link with existing connection", 0, 0, 0, 0, 1);
//    QVERIFY2(p->actionLink().connection() == c, "Connection not set in new link after redo");

//    GGContentElement *e1 = new GGImageContent;
//    QVERIFY2(m_stk->execute(m_fac->exchangeContent(p, e1)), "Cannot set content");
//    VERIFYSIG(m_sc, "Set content", 0, 0, 0, 0, 1);
//    QVERIFY2(p->content() == e1, "Content not set");
//    QVERIFY2(m_stk->undo(), "Cannot undo set content");
//    VERIFYSIG(m_sc, "Undo Set content", 0, 0, 0, 0, 1);
//    QVERIFY2(p->content() == NULL, "Content still set");
//    QVERIFY2(m_stk->redo(), "Cannot redo set content");
//    VERIFYSIG(m_sc, "Redo Set content", 0, 0, 0, 0, 1);
//    QVERIFY2(p->content() == e1, "Content not set");

//    GGContentElement *e2 = new GGImageContent;
//    QVERIFY2(m_stk->execute(m_fac->exchangeContent(p, e2)), "Cannot set content");
//    VERIFYSIG(m_sc, "Exchange content", 0, 0, 0, 0, 1);
//    QVERIFY2(p->content() == e2, "Content not updated");
//    QVERIFY2(m_stk->undo(), "Cannot undo set content");
//    VERIFYSIG(m_sc, "Undo Set content", 0, 0, 0, 0, 1);
//    QVERIFY2(p->content() == e1, "Content still set");
//    QVERIFY2(m_stk->redo(), "Cannot redo set content");
//    VERIFYSIG(m_sc, "Redo Set content", 0, 0, 0, 0, 1);
//    QVERIFY2(p->content() == e2, "Content not set");

//    QVERIFY2(m_stk->execute(m_fac->exchangeContent(p, e2)), "Cannot set content");
//    VERIFYSIGNULL(m_sc, "Setting same content resulted in signal");

//}

//void GGEditor_ModelCommandTest::testMappedLinkCommands()
//{
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createActionPage());
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createEndPage());
//    GGMappedContentPage *p = ggpage_cast<GGActionPage *> (m_model->getPages()[0]);
//    GGPage *e1 = m_model->getPages()[1];
//    m_sc->reset();

//    QVERIFY2(!m_stk->execute(m_fac->setMappedLink(p, 0, GGMappedLink::rectangle(QRect()))), "Can set mapped link that doesn't exist");
//    VERIFYSIGNULL(m_sc, "Setting invalid mapped link resulted in signal");

//    QVERIFY2(m_stk->execute(m_fac->addMappedLink(p)), "Cannot add mapped link");
//    VERIFYSIG(m_sc, "Add mapped link", 0, 0, 0, 0, 1);
//    QVERIFY(p->getLinkMap().size() == 1);

//    GGMappedLink ml = GGMappedLink::rectangle(QRect());
//    GGLink l;
//    GGConnection *conn = m_model->factory()->createConnection(p->id(), e1->id());
//    l.setConnection(conn);
//    ml.setLink(l);
//    QVERIFY2(!m_stk->execute(m_fac->setMappedLink(p, 0, ml)), "Can set mapped link with connection");
//    VERIFYSIGNULL(m_sc, "Setting mapped link with connection resulted in signal");
//    // Prevent later bugs by reusing these
//    l.setConnection(NULL);
//    ml = GGMappedLink::rectangle(QRect());
//    delete conn;
//    conn = NULL;

//    QVERIFY2(m_stk->undo(), "Cannot undo adding mapped link");
//    VERIFYSIG(m_sc, "Undo Add mapped link", 0, 0, 0, 0, 1);
//    QVERIFY(p->getLinkMap().isEmpty());
//    QVERIFY2(m_stk->redo(), "Cannot redo adding mapped link");
//    VERIFYSIG(m_sc, "Redo Add mapped link", 0, 0, 0, 0, 1);
//    QVERIFY(p->getLinkMap().size() == 1);

//    QVERIFY2(m_stk->execute(m_fac->createConnection(p, e1, GGConnectionSlot(GGConnectionSlot::MappedConnection, 0))), "Cannot set Mapped connection");
//    VERIFYSIG(m_sc, "Create Mapped connection", 0, 0, 1, 0, 1);
//    QVERIFY2(p->getLinkMap()[0].link().connection() != NULL, "No connection set in mapped link");
//    QVERIFY2(m_stk->undo(), "Cannot undo set Mapped connection");
//    VERIFYSIG(m_sc, "Undo Create Mapped connection", 0, 0, 0, 1, 1);
//    QVERIFY2(p->getLinkMap()[0].link().connection() == NULL, "connection still set in mapped link after undo");
//    QVERIFY2(m_stk->redo(), "Cannot redo set Mapped connection");
//    VERIFYSIG(m_sc, "Redo Create Mapped connection", 0, 0, 1, 0, 1);
//    QVERIFY2(p->getLinkMap()[0].link().connection() != NULL, "No connection set in mapped link after redo");

//    // RESET
//    QVERIFY2(m_stk->execute(m_fac->setMappedLink(p, 0, GGMappedLink::rectangle(QRect()))), "Cannot reset mapped link");
//    VERIFYSIG(m_sc, "Reset mapped link", 0, 0, 0, 0, 1);
//    QVERIFY2(p->getLinkMap()[0].link().connection() != NULL, "No connection set in mapped link after reset");
//    QVERIFY2(m_stk->undo(), "Cannot undo reset mapped link");
//    VERIFYSIG(m_sc, "Undo Reset mapped link", 0, 0, 0, 0, 1);
//    QVERIFY2(p->getLinkMap()[0].link().connection() != NULL, "No connection set in mapped link after undo reset");
//    QVERIFY2(m_stk->redo(), "Cannot redo reset mapped link");
//    VERIFYSIG(m_sc, "Redo Reset mapped link", 0, 0, 0, 0, 1);
//    QVERIFY2(p->getLinkMap()[0].link().connection() != NULL, "No connection set in mapped link after redo reset");

//    // REMOVE
//    QVERIFY2(!m_stk->execute(m_fac->removeMappedLink(p, 1)), "Can remove nonexisting mapped link");
//    VERIFYSIGNULL(m_sc, "Remove nonexisting mapped link");

//    QVERIFY2(m_stk->execute(m_fac->removeMappedLink(p, 0)), "Cannot remove mapped link");
//    VERIFYSIG(m_sc, "Remove mapped link", 0, 0, 0, 1, 1);
//}

//void GGEditor_ModelCommandTest::testDecisionLinkCommands()
//{
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createDecisionPage());
//    GGAbstractCommandFactory::oneShotCommand(m_fac->createEndPage());
//    GGDecisionPage *p = ggpage_cast<GGDecisionPage *> (m_model->getPages()[0]);
//    GGPage *e1 = m_model->getPages()[1];
//    m_sc->reset();

//    QVERIFY2(!m_stk->execute(m_fac->setDecisionLink(p, 0, GGLink())), "Can set decision link that doesn't exist");
//    VERIFYSIGNULL(m_sc, "Setting invalid decision link resulted in signal");

//    QVERIFY2(m_stk->execute(m_fac->addDecisionLink(p)), "Cannot add decision link");
//    VERIFYSIG(m_sc, "Add decision link", 0, 0, 0, 0, 1);
//    QVERIFY(p->getDecisionLinks().size() == 1);

//    GGLink l;
//    GGConnection *conn = m_model->factory()->createConnection(p->id(), e1->id());
//    l.setConnection(conn);
//    QVERIFY2(!m_stk->execute(m_fac->setDecisionLink(p, 0, l)), "Can set decision link with connection");
//    VERIFYSIGNULL(m_sc, "Setting decision link with connection resulted in signal");
//    // Prevent later bugs by reusing these
//    l.setConnection(NULL);
//    delete conn;
//    conn = NULL;

//    QVERIFY2(m_stk->undo(), "Cannot undo adding decision link");
//    VERIFYSIG(m_sc, "Undo Add decision link", 0, 0, 0, 0, 1);
//    QVERIFY(p->getDecisionLinks().isEmpty());
//    QVERIFY2(m_stk->redo(), "Cannot redo adding decision link");
//    VERIFYSIG(m_sc, "Redo Add decision link", 0, 0, 0, 0, 1);
//    QVERIFY(p->getDecisionLinks().size() == 1);

//    QVERIFY2(m_stk->execute(m_fac->createConnection(p, e1, GGConnectionSlot(GGConnectionSlot::DecisionConnection, 0))), "Cannot set decision connection");
//    VERIFYSIG(m_sc, "Create decision connection", 0, 0, 1, 0, 1);
//    QVERIFY2(p->getDecisionLinks()[0].connection() != NULL, "No connection set in decision link");
//    QVERIFY2(m_stk->undo(), "Cannot undo set decision connection");
//    VERIFYSIG(m_sc, "Undo Create decision connection", 0, 0, 0, 1, 1);
//    QVERIFY2(p->getDecisionLinks()[0].connection() == NULL, "connection still set in decision link after undo");
//    QVERIFY2(m_stk->redo(), "Cannot redo set decision connection");
//    VERIFYSIG(m_sc, "Redo Create decision connection", 0, 0, 1, 0, 1);
//    QVERIFY2(p->getDecisionLinks()[0].connection() != NULL, "No connection set in decision link after redo");

//    // RESET
//    QVERIFY2(m_stk->execute(m_fac->setDecisionLink(p, 0, GGLink())), "Cannot reset decision link");
//    VERIFYSIG(m_sc, "Reset decision link", 0, 0, 0, 0, 1);
//    QVERIFY2(p->getDecisionLinks()[0].connection() != NULL, "No connection set in decision link after reset");
//    QVERIFY2(m_stk->undo(), "Cannot undo reset decision link");
//    VERIFYSIG(m_sc, "Undo Reset decision link", 0, 0, 0, 0, 1);
//    QVERIFY2(p->getDecisionLinks()[0].connection() != NULL, "No connection set in decision link after undo reset");
//    QVERIFY2(m_stk->redo(), "Cannot redo reset decision link");
//    VERIFYSIG(m_sc, "Redo Reset decision link", 0, 0, 0, 0, 1);
//    QVERIFY2(p->getDecisionLinks()[0].connection() != NULL, "No connection set in decision link after redo reset");

//    // REMOVE
//    QVERIFY2(!m_stk->execute(m_fac->removeDecisionLink(p, 1)), "Can remove nonexisting decision link");
//    VERIFYSIGNULL(m_sc, "Remove nonexisting decision link");

//    QVERIFY2(m_stk->execute(m_fac->removeDecisionLink(p, 0)), "Cannot remove decision link");
//    VERIFYSIG(m_sc, "Remove decision link", 0, 0, 0, 1, 1);
//}
