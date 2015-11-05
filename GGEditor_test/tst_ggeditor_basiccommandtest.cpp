#include "tst_ggeditor_basiccommandtest.h"

#include <command/ggeditcommandfactory.h>
#include <command/ggmodelcommands.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>

void GGEditor_BasicCommandTest::testCommandBasics()
{
    struct DummyCommand : public GGAbstractCommand {
       DummyCommand(bool ret) : GGAbstractCommand(), ret(ret){}
       bool ret;
       QString description() const { return ""; }
    protected:
        bool doExecute() { return ret; }
        bool doUndo() { return ret; }
        bool doRedo() { return ret; }
    };

    DummyCommand cmd(false);

    QVERIFY2(cmd.state() == GGAbstractCommand::NotExecuted, "New Command is not in NotExecuted state");
    QVERIFY2(cmd.undo() == false, "Can Undo new command");
    QVERIFY2(cmd.redo() == false, "Can Redo new command");
    QVERIFY2(cmd.execute() == false, "Can execute failing command");

    cmd.ret = true;
    QVERIFY2(cmd.execute(), "Cannot execute successful command");
    QVERIFY2(cmd.state() == GGAbstractCommand::Executed, "Executed Command is not in Executed state");
    QVERIFY2(cmd.redo() == false, "Can Redo executed command");
    QVERIFY2(cmd.execute() == false, "Can execute executed command");
    cmd.ret = false;
    QVERIFY2(cmd.undo() == false, "Can undo failing command");

    cmd.ret = true;
    QVERIFY2(cmd.undo(), "Cannot undo succssful command");
    QVERIFY2(cmd.state() == GGAbstractCommand::Undone, "Undone Command is not in Undone state");
    QVERIFY2(cmd.undo() == false, "Can Undo undone command");
    QVERIFY2(cmd.execute() == false, "Can execute undone command");
    cmd.ret = false;
    QVERIFY2(cmd.redo() == false, "Can redo failing command");

    cmd.ret = true;
    QVERIFY2(cmd.redo(), "Cannot redo succssful command");
    QVERIFY2(cmd.state() == GGAbstractCommand::Executed, "Redone Command is not in Executed state");
    QVERIFY2(cmd.redo() == false, "Can Redo redone command");
    QVERIFY2(cmd.execute() == false, "Can execute redone command");
}

void GGEditor_BasicCommandTest::testCreatePage()
{
    GGEditModel model(new GGSimpleFactory);
    GGEditCommandFactory fac(&model);
    GGCreatePageCmd *cmd = fac.createStartPage();

    QVERIFY2(cmd->execute(), qPrintable("Error executing CreatePage command: " + cmd->error()));

    QVERIFY2(cmd->createdPage() != NULL, "CreatePage command has no created page");
    QVERIFY2(cmd->createdPage()->model() == &model, "Created page is not associated with the model");
    QVERIFY2(model.getPage(cmd->createdPage()->id()) == cmd->createdPage(), "Created Page is not same as retrieved from model");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing CreatePage command: " + cmd->error()));
    QVERIFY2(cmd->createdPage()->model() == NULL, "Created page is still associated with model after undo");
    QVERIFY2(model.getPage(cmd->createdPage()->id()) == NULL, "Created Page is in model after undo");

    QVERIFY2(cmd->redo(), qPrintable("Error redoing CreatePage command: " + cmd->error()));
    QVERIFY2(cmd->createdPage()->model() == &model, "Created page is not associated with the model after redo");
    QVERIFY2(model.getPage(cmd->createdPage()->id()) == cmd->createdPage(), "Created Page is not same as retrieved from model after redo");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice CreatePage command: " + cmd->error()));
    QVERIFY2(cmd->createdPage()->model() == NULL, "Created page is still associated with model after second undo");
    QVERIFY2(model.getPage(cmd->createdPage()->id()) == NULL, "Created Page is in model after second undo");
}

void GGEditor_BasicCommandTest::testDeletePage()
{
    GGEditModel model(new GGSimpleFactory);
    GGEditCommandFactory fac(&model);
    GGCreatePageCmd *cr = fac.createStartPage();
    cr->execute();
    GGPage *p = cr->createdPage();
    delete cr;

    GGDeletePageCmd *cmd = fac.deletePage(p);

    QVERIFY2(p == cmd->deletedPage(), "Deleted page is not the page to delete");

    QVERIFY2(cmd->execute(), qPrintable("Error executing DeletePage command: " + cmd->error()));
    QVERIFY2(p->model() == NULL, "Page is still associated with model after delete");
    QVERIFY2(model.getPage(p->id()) == NULL, "Page is still in model after delete");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing DeletePage command: " + cmd->error()));
    QVERIFY2(cmd->deletedPage()->model() == &model, "Deleted page is not associated with the model after undo");
    QVERIFY2(model.getPage(cmd->deletedPage()->id()) == cmd->deletedPage(), "Deleted Page is not same as retrieved from model after undo");

    QVERIFY2(cmd->redo(), qPrintable("Error redoing DeletePage command: " + cmd->error()));
    QVERIFY2(p->model() == NULL, "Page is still associated with model after redo");
    QVERIFY2(model.getPage(p->id()) == NULL, "Page is still in model after redo");

    QVERIFY2(cmd->undo(), qPrintable("Error undoing twice DeletePage command: " + cmd->error()));
    QVERIFY2(cmd->deletedPage()->model() == &model, "Deleted page is not associated with the model after second undo");
    QVERIFY2(model.getPage(cmd->deletedPage()->id()) == cmd->deletedPage(), "Deleted Page is not same as retrieved from model after second undo");

    delete cmd;
}

void GGEditor_BasicCommandTest::testDeletePageWithConnection()
{
    GGEditModel model(new GGSimpleFactory);
    GGEditCommandFactory fac(&model);
    GGCreatePageCmd *cr = fac.createStartPage();
    cr->execute();
    GGStartPage *p1 = ggpage_cast<GGStartPage*>(cr->createdPage());
    delete cr;
    cr = fac.createEndPage();
    cr->execute();
    GGPage *p2 = cr->createdPage();
    delete cr;

    // TODO: CreateConnectionCmd
    QWARN("Start Connection is set manually. Replace by command in future");
    GGConnection *c = model.factory()->createConnection(p1->id(), p2->id());
    model.registerNewConnection(c);
    p1->setStartConnection(c);

    GGDeletePageCmd *cmd = fac.deletePage(p1);
    QVERIFY2(cmd->execute(), "Error deleting src");
    QVERIFY2(c->model() == NULL, "Connection still associated with model after delete src");
    QVERIFY2(model.getConnection(c->id()) == NULL, "Connection is still in model after delete src");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after delete src");
    QVERIFY2(model.getPageIncommingConnections(p2).isEmpty(), "Dest still has incomming connections after delete src");

    QVERIFY2(cmd->undo(), "Error undoing delete of src");
    QVERIFY2(c->model() == &model, "Connection not associated with model after undo delete src");
    QVERIFY2(model.getConnection(c->id()) == c, "Connection is still in model after undo delete src");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after undo delete src");
    QVERIFY2(model.getPageIncommingConnections(p2).size() == 1, "Dest has no incomming connections after undo delete src");

    QVERIFY2(cmd->redo(), "Error redoing delete of src");
    QVERIFY2(c->model() == NULL, "Connection still associated with model after redo delete src");
    QVERIFY2(model.getConnection(c->id()) == NULL, "Connection is still in model after redo delete src");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after redo delete src");
    QVERIFY2(model.getPageIncommingConnections(p2).isEmpty(), "Dest still has incomming connections after redo delete src");

    QVERIFY2(cmd->undo(), "Error undoing twice delete of src");
    QVERIFY2(c->model() == &model, "Connection not associated with model after second undo delete src");
    QVERIFY2(model.getConnection(c->id()) == c, "Connection is still in model after second undo delete src");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after second undo delete src");
    QVERIFY2(model.getPageIncommingConnections(p2).size() == 1, "Dest has no incomming connections after second undo delete src");
    delete cmd;

    cmd = fac.deletePage(p2);
    QVERIFY2(cmd->execute(), "Error deleting dest");
    QVERIFY2(c->model() == NULL, "Connection still associated with model after delete dest");
    QVERIFY2(model.getConnection(c->id()) == NULL, "Connection is still in model after delete dest");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after delete dest");

    QVERIFY2(cmd->undo(), "Error undoing delete of dest");
    QVERIFY2(c->model() == &model, "Connection not associated with model after undo delete dest");
    QVERIFY2(model.getConnection(c->id()) == c, "Connection is still in model after undo delete dest");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after undo delete dest");

    QVERIFY2(cmd->redo(), "Error redoing delete of dest");
    QVERIFY2(c->model() == NULL, "Connection still associated with model after redo delete dest");
    QVERIFY2(model.getConnection(c->id()) == NULL, "Connection is still in model after redo delete dest");
    QVERIFY2(p1->startConnection() == NULL, "Src still has Start Connection after redo delete dest");

    QVERIFY2(cmd->undo(), "Error undoing twice delete of dest");
    QVERIFY2(c->model() == &model, "Connection not associated with model after second undo delete dest");
    QVERIFY2(model.getConnection(c->id()) == c, "Connection is still in model after second undo delete dest");
    QVERIFY2(p1->startConnection() == c, "Start Connection not set correctly after second undo delete dest");
    delete cmd;
}
