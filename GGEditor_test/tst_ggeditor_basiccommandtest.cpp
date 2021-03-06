#include "tst_ggeditor_basiccommandtest.h"

#include <command/ggcommandstack.h>
#include <command/ggcommandgroup.h>

struct DummyCommand : public GGAbstractCommand {
   DummyCommand(bool ret = true, QString desc = "Dummy") : GGAbstractCommand(), ret(ret), desc(desc) {}
   bool ret;
   QString desc;
   QString description() const { return desc; }
protected:
    bool doExecute() { return ret ? true : setError(desc + " failed execute"); }
    bool doUndo() { return ret ? true : setError(desc + " failed undo"); }
    bool doRedo() { return ret ? true : setError(desc + " failed redo"); }
};

struct Incer : public DummyCommand {
    Incer(int &i, bool ret = true, QString desc = "Incer") : DummyCommand(ret, desc), i(i) {}
    int &i;
protected:
    bool doExecute() { if (DummyCommand::doExecute()) { i+=5; return true; } return false;  }
    bool doUndo() { if (DummyCommand::doUndo()) { i-=5; return true; } return false; }
    bool doRedo() { if (DummyCommand::doRedo()) { i+=5; return true; } return false; }
};

struct Doubler : public DummyCommand {
    Doubler(int &i, bool ret = true, QString desc = "Doubler") : DummyCommand(ret, desc), i(i) {}
    int &i;
protected:
    bool doExecute() { if (DummyCommand::doExecute()) { i*=2; return true; } return false;  }
    bool doUndo() { if (DummyCommand::doUndo()) { i/=2; return true; } return false; }
    bool doRedo() { if (DummyCommand::doRedo()) { i*=2; return true; } return false; }
};

////////////////////////////

void GGEditor_BasicCommandTest::testCommandBasics()
{

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

void GGEditor_BasicCommandTest::testCommandStack()
{
    QList<DummyCommand *> lst;
    lst << new DummyCommand(true) << new DummyCommand(true) << new DummyCommand(true) << new DummyCommand(true);

    GGCommandStack stk;
    QVERIFY2(stk.lastCommand() == NULL, "Initial stack is wrong");
    QVERIFY2(stk.undoCommand() == NULL, "Initial stack is wrong");
    QVERIFY2(stk.redoCommand() == NULL, "Initial stack is wrong");
    QVERIFY2(stk.getAllCommands().isEmpty(), "Initial stack is wrong");
    QVERIFY2(stk.getUndoCommands().isEmpty(), "Initial stack is wrong");
    QVERIFY2(stk.getRedoCommands().isEmpty(), "Initial stack is wrong");

    QVERIFY2(stk.execute(lst[0]), "Error in execute 1st command");
    QVERIFY2(stk.lastCommand() == lst[0], "Last wrong after 1st command");
    QVERIFY2(stk.undoCommand() == lst[0], "Undo wrong after 1st command");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after 1st command");

    QVERIFY2(stk.execute(lst[1]), "Error in execute 2nd command");
    QVERIFY2(stk.lastCommand() == lst[1], "Last wrong after 2nd command");
    QVERIFY2(stk.undoCommand() == lst[1], "Undo wrong after 2nd command");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after 2nd command");

    QVERIFY2(stk.execute(lst[2]), "Error in execute 3rd command");
    QVERIFY2(stk.lastCommand() == lst[2], "Last wrong after 3rd command");
    QVERIFY2(stk.undoCommand() == lst[2], "Undo wrong after 3rd command");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after 3rd command");

    QVERIFY2(!stk.redo(), "Can Redo although none undone");

    QVERIFY2(stk.undo(), "Cannot undo");
    QVERIFY2(stk.lastCommand() == lst[2], "Wrong last after undo");
    QVERIFY2(stk.undoCommand() == lst[1], "Wrong undo after undo");
    QVERIFY2(stk.redoCommand() == lst[2], "Wrong redo after undo");

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after undo");
    QVERIFY2(stk.getRedoCommands().size() == 1, "Wrong redo size after undo");
    QVERIFY2(stk.getRedoCommands()[0] == lst[2], "Wrong redo commands after undo");
    QVERIFY2(stk.getUndoCommands().size() == 2, "Wrong undo size after undo");
    QVERIFY2(stk.getUndoCommands()[0] == lst[1], "Wrong undo commands after undo");
    QVERIFY2(stk.getUndoCommands()[1] == lst[0], "Wrong undo commands after undo");

    QVERIFY2(stk.execute(lst[3]), "Cannot execute after undo");
    QVERIFY2(stk.lastCommand() == lst[3], "Last wrong after 4th command");
    QVERIFY2(stk.undoCommand() == lst[3], "Undo wrong after 4th command");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after 4th command");


    QVERIFY2(!stk.redo(), "Can Redo although purged");

    QVERIFY2(stk.undo(), "Cannot undo");
    QVERIFY2(stk.lastCommand() == lst[3], "Wrong last after undo");
    QVERIFY2(stk.undoCommand() == lst[1], "Wrong undo after undo");
    QVERIFY2(stk.redoCommand() == lst[3], "Wrong redo after undo");

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after undo");
    QVERIFY2(stk.getRedoCommands().size() == 1, "Wrong redo size after undo");
    QVERIFY2(stk.getRedoCommands()[0] == lst[3], "Wrong redo commands after undo");
    QVERIFY2(stk.getUndoCommands().size() == 2, "Wrong undo size after undo");
    QVERIFY2(stk.getUndoCommands()[0] == lst[1], "Wrong undo commands after undo");
    QVERIFY2(stk.getUndoCommands()[1] == lst[0], "Wrong undo commands after undo");

    QVERIFY2(stk.redo(), "Cannot redo");
    QVERIFY2(stk.lastCommand() == lst[3], "Last wrong after 4th command");
    QVERIFY2(stk.undoCommand() == lst[3], "Undo wrong after 4th command");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after 4th command");

    /// UNDO ALL
    QVERIFY2(stk.undo(), "Cannot undo");
    QVERIFY2(stk.lastCommand() == lst[3], "Wrong last after undo");
    QVERIFY2(stk.undoCommand() == lst[1], "Wrong undo after undo");
    QVERIFY2(stk.redoCommand() == lst[3], "Wrong redo after undo");

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after undo");
    QVERIFY2(stk.getRedoCommands().size() == 1, "Wrong redo size after undo");
    QVERIFY2(stk.getRedoCommands()[0] == lst[3], "Wrong redo commands after undo");
    QVERIFY2(stk.getUndoCommands().size() == 2, "Wrong undo size after undo");
    QVERIFY2(stk.getUndoCommands()[0] == lst[1], "Wrong undo commands after undo");
    QVERIFY2(stk.getUndoCommands()[1] == lst[0], "Wrong undo commands after undo");


    QVERIFY2(stk.undo(), "Cannot undo");
    QVERIFY2(stk.lastCommand() == lst[1], "Wrong last after undo");
    QVERIFY2(stk.undoCommand() == lst[0], "Wrong undo after undo");
    QVERIFY2(stk.redoCommand() == lst[1], "Wrong redo after undo");

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after undo");
    QVERIFY2(stk.getRedoCommands().size() == 2, "Wrong redo size after undo");
    QVERIFY2(stk.getRedoCommands()[0] == lst[1], "Wrong redo commands after undo");
    QVERIFY2(stk.getRedoCommands()[1] == lst[3], "Wrong undo commands after undo");
    QVERIFY2(stk.getUndoCommands().size() == 1, "Wrong undo size after undo");
    QVERIFY2(stk.getUndoCommands()[0] == lst[0], "Wrong undo commands after undo");

    QVERIFY2(stk.undo(), "Cannot undo");
    QVERIFY2(stk.lastCommand() == lst[0], "Wrong last after undo");
    QVERIFY2(stk.undoCommand() == NULL, "Wrong undo after undo");
    QVERIFY2(stk.redoCommand() == lst[0], "Wrong redo after undo");

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after undo");
    QVERIFY2(stk.getRedoCommands().size() == 3, "Wrong redo size after undo");
    QVERIFY2(stk.getRedoCommands()[0] == lst[0], "Wrong undo commands after undo");
    QVERIFY2(stk.getRedoCommands()[1] == lst[1], "Wrong redo commands after undo");
    QVERIFY2(stk.getRedoCommands()[2] == lst[3], "Wrong undo commands after undo");
    QVERIFY2(stk.getUndoCommands().isEmpty(), "Wrong undo size after undo");

    QVERIFY2(!stk.undo(), "Can undo although empty");

    QVERIFY(stk.redo());
    QVERIFY(stk.redo());
    QVERIFY(stk.redo());
    QVERIFY2(stk.lastCommand() == lst[3], "Last wrong after redoing all");
    QVERIFY2(stk.undoCommand() == lst[3], "Undo wrong after redoing all");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after redoing all");

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after redoing all");
    QVERIFY2(stk.getRedoCommands().isEmpty(), "Wrong redo size after redoing all");
    QVERIFY2(stk.getUndoCommands().size() == 3, "Wrong undo size after redoing all");
    QVERIFY2(stk.getUndoCommands()[0] == lst[3], "Wrong undo commands after redoing all");
    QVERIFY2(stk.getUndoCommands()[1] == lst[1], "Wrong redo commands after redoing all");
    QVERIFY2(stk.getUndoCommands()[2] == lst[0], "Wrong undo commands after redoing all");

    QVERIFY(stk.undo());
    QVERIFY(stk.undo());
    QVERIFY(stk.undo());

    QVERIFY2(stk.getAllCommands().size() == 3, "Wrong size after failed command");
    QVERIFY2(stk.getUndoCommands().size() == 0, "Wrong redo size after failed command");
    QVERIFY2(stk.getRedoCommands().size() == 3, "Wrong size after failed command");
    QVERIFY2(stk.getRedoCommands()[0] == lst[0], "Wrong undo commands after redoing all");
    QVERIFY2(stk.getRedoCommands()[1] == lst[1], "Wrong redo commands after redoing all");
    QVERIFY2(stk.getRedoCommands()[2] == lst[3], "Wrong undo commands after redoing all");

}

void GGEditor_BasicCommandTest::testCommandStackWithFails()
{
    QList<DummyCommand *> lst;
    lst << new DummyCommand(false) << new DummyCommand(false) << new DummyCommand(false);

    GGCommandStack stk;
    QVERIFY2(!stk.execute(lst[0], false), "Can execute failed");
    QVERIFY2(stk.lastCommand() == NULL, "Last wrong after failed");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after failed");
    QVERIFY2(stk.undoCommand() == NULL, "Undo wrong after failed");
    QVERIFY2(stk.getRedoCommands().isEmpty(), "Redo list wrong after failed");
    QVERIFY2(stk.getUndoCommands().isEmpty(), "Undo list wrong after failed");

    lst[0]->ret = true;
    QVERIFY(stk.execute(lst[0]));

    QVERIFY2(!stk.execute(lst[1], false), "Can execute failed");
    QVERIFY2(stk.lastCommand() == lst[0], "Last wrong after failed");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after failed");
    QVERIFY2(stk.undoCommand() == lst[0], "Undo wrong after failed");
    QVERIFY2(stk.getRedoCommands().isEmpty(), "Redo list wrong after failed");
    QVERIFY2(stk.getUndoCommands().size() == 1, "Undo list wrong after failed");
    QVERIFY2(stk.getUndoCommands()[0] == lst[0], "Undo list wrong after failed");

    lst[1]->ret = true;
    QVERIFY(stk.execute(lst[1]));

    lst[1]->ret = false;
    QVERIFY2(!stk.undo(), "Can undo failed");
    // Stack not modified after failed undo Last correct was lst[1]
    QVERIFY2(stk.lastCommand() == lst[1], "Last wrong after failed");
    QVERIFY2(stk.redoCommand() == NULL, "Redo wrong after failed");
    QVERIFY2(stk.undoCommand() == lst[1], "Undo wrong after failed");
    QVERIFY2(stk.getRedoCommands().isEmpty(), "Redo list wrong after failed");
    QVERIFY2(stk.getUndoCommands().size() == 2, "Undo list wrong after failed");
    QVERIFY2(stk.getUndoCommands()[0] == lst[1], "Undo list wrong after failed");
    QVERIFY2(stk.getUndoCommands()[1] == lst[0], "Undo list wrong after failed");

    lst[1]->ret = true;
    QVERIFY(stk.undo());

    lst[1]->ret = false;

    QVERIFY(stk.undo());
    QVERIFY(stk.redo());

    // Redoing will fail now
    QVERIFY2(!stk.redo(), "Can redo failed");
    // Stack not modified after failed redo Last correct was lst[0]
    QVERIFY2(stk.lastCommand() == lst[0], "Last wrong after failed");
    QVERIFY2(stk.redoCommand() == lst[1], "Redo wrong after failed");
    QVERIFY2(stk.undoCommand() == lst[0], "Undo wrong after failed");
    QVERIFY2(stk.getRedoCommands().size() == 1, "Redo list wrong after failed");
    QVERIFY2(stk.getRedoCommands()[0] == lst[1], "Undo list wrong after failed");
    QVERIFY2(stk.getUndoCommands().size() == 1, "Undo list wrong after failed");
    QVERIFY2(stk.getUndoCommands()[0] == lst[0], "Undo list wrong after failed");

    // must purge on failed execute
    QVERIFY(!stk.execute(lst[2])); // lst[2] is not pushed on stack, delete it if failed!
    QVERIFY(stk.lastCommand() == lst[0]);
    QVERIFY(stk.redoCommand() == NULL);
    QVERIFY(stk.undoCommand() == lst[0]);
    QVERIFY(stk.getRedoCommands().isEmpty());
    QVERIFY(stk.getUndoCommands().size() == 1);
    QVERIFY(stk.getUndoCommands()[0] == lst[0]);
}

GGCommandGroup *getIncDoubleGroup(int &i)
{
    GGCommandGroup *grp = new GGCommandGroup;
    Incer *ig = new Incer(i);
    Doubler *dg = new Doubler(i);
    grp->addCommand(ig);
    grp->addCommand(dg);
    return grp;
}

void setGrpDummyRet(GGCommandGroup *grp, int idx, bool ret)
{
    static_cast<DummyCommand*>(grp->commands()[idx])->ret = ret;
}

void GGEditor_BasicCommandTest::testCommandGroupOrder()
{
    int i = 10;

    // Test with all success
    GGCommandGroup *grp = getIncDoubleGroup(i);
    QVERIFY(grp->execute());
    QVERIFY2(i == 30, "Group Command execute sequence not correct");
    QVERIFY(grp->undo());
    QVERIFY2(i == 10, "Group Command undo sequence not correct");
    QVERIFY(grp->redo());
    QVERIFY2(i == 30, "Group Command redo sequence not correct");
    delete grp;

    // Test with first fail
    grp = getIncDoubleGroup(i);
    i = 10;
    setGrpDummyRet(grp, 0, false);
    QVERIFY(!grp->execute());
    QVERIFY2(i == 10, "i changed after failing group execute");
    // Let it work
    setGrpDummyRet(grp, 0, true);
    QVERIFY(grp->execute());
    QVERIFY(i == 30);

    setGrpDummyRet(grp, 0, false);
    QVERIFY(!grp->undo());
    QVERIFY2(i == 30, "i changed after failing group undo");
    // Let it work
    setGrpDummyRet(grp, 0, true);
    QVERIFY(grp->undo());
    QVERIFY(i == 10);

    setGrpDummyRet(grp, 0, false);
    QVERIFY(!grp->redo());
    QVERIFY2(i == 10, "i changed after failing group redo");
    // Let it work
    setGrpDummyRet(grp, 0, true);
    QVERIFY(grp->redo());
    QVERIFY(i == 30);
    delete grp;

    // Test with second fail
    grp = getIncDoubleGroup(i);
    i = 10;
    setGrpDummyRet(grp, 1, false);
    QVERIFY(!grp->execute());
    QVERIFY2(i == 10, "i changed after failing group execute");
    // Let it work
    setGrpDummyRet(grp, 1, true);
    QVERIFY(grp->execute());
    QVERIFY(i == 30);

    setGrpDummyRet(grp, 1, false);
    QVERIFY(!grp->undo());
    QVERIFY2(i == 30, "i changed after failing group undo");
    // Let it work
    setGrpDummyRet(grp, 1, true);
    QVERIFY(grp->undo());
    QVERIFY(i == 10);

    setGrpDummyRet(grp, 1, false);
    QVERIFY(!grp->redo());
    QVERIFY2(i == 10, "i changed after failing group redo");
    // Let it work
    setGrpDummyRet(grp, 1, true);
    QVERIFY(grp->redo());
    QVERIFY(i == 30);
    delete grp;
}

void GGEditor_BasicCommandTest::testCommandStackOrder()
{
    int i = 10;
    Incer *inc = new Incer(i);
    Doubler *dbl = new Doubler(i);
    GGCommandStack stk;

    QVERIFY(stk.execute(inc));
    QVERIFY(i == 15);
    QVERIFY(stk.execute(dbl));
    QVERIFY(i == 30);

    QVERIFY(stk.undo());
    QVERIFY(i == 15);
    QVERIFY(stk.undo());
    QVERIFY(i == 10);

    QVERIFY(stk.redo());
    QVERIFY(i == 15);
    QVERIFY(stk.redo());
    QVERIFY(i == 30);

    QVERIFY(stk.undo());
    QVERIFY(stk.execute(new Incer(i)));
    QVERIFY(i == 20);
    QVERIFY(!stk.redo());
    QVERIFY(i == 20);
}

void GGEditor_BasicCommandTest::testCommandStackMerge()
{
    int i = 10;
    //Incer *inc1 = new Incer(i);
    //Doubler *dbl1 = new Doubler(i);
    GGCommandStack stk1;

    QVERIFY(stk1.execute(new Incer(i)));
    QVERIFY(stk1.execute(new Doubler(i)));
    QVERIFY(stk1.execute(new Doubler(i)));
    QVERIFY(stk1.undo());
    QVERIFY(i == 30);

    {
        GGCommandStack stk2;
        QVERIFY(stk2.execute(new Incer(i)));
        QVERIFY(stk2.execute(new Doubler(i)));
        QVERIFY(i == 70);
        stk1.merge(stk2);
        QVERIFY2(stk1.getUndoCommands().size() == 4, "Stack size not correct after merge");
    }

    {
        GGCommandStack stk2;
        QVERIFY(stk2.execute(new Incer(i)));
        QVERIFY(stk2.execute(new Doubler(i)));
        QVERIFY(stk2.execute(new Doubler(i)));
        QVERIFY(stk2.undo());
        QVERIFY(i == 150);
        stk1.merge(stk2);
        QVERIFY2(stk1.getUndoCommands().size() == 6, "Stack size not correct after merge with stack having undone command");
    }

    {
        GGCommandStack stk2;
        // For redoability
        stk1.execute(new Incer(i));
        stk1.undo();

        QVERIFY(i == 150);
        stk1.merge(stk2);
        QVERIFY2(stk1.getUndoCommands().size() == 6, "Stack size not correct after merge with empty stack");
        QVERIFY2(stk1.redoCommand() != NULL, "Redoability gone after merging with empty stack");
    }

    {
        GGCommandStack stk2;
        // For redoability
        stk1.execute(new Incer(i));
        stk1.undo();

        QVERIFY(stk2.execute(new Incer(i)));
        QVERIFY(stk2.execute(new Doubler(i)));
        QVERIFY(stk2.undo());
        QVERIFY(stk2.undo());
        QVERIFY(i == 150);
        stk1.merge(stk2);
        QVERIFY2(stk1.getUndoCommands().size() == 6, "Stack size not correct after merge with stack having all commands undone");
        QVERIFY2(stk1.redoCommand() != NULL, "Redoability gone after merging with empty stack");
    }

    QVERIFY(stk1.undo());
    QVERIFY(stk1.undo());
    QVERIFY(stk1.undo());
    QVERIFY(stk1.undo());
    QVERIFY(stk1.undo());
    QVERIFY(stk1.undo());
    QVERIFY(i == 10);

}

void GGEditor_BasicCommandTest::testCommandGroupFromStack()
{
    int i = 10;
    Incer *inc = new Incer(i);
    Doubler *dbl = new Doubler(i);
    GGCommandGroup *grp = NULL;

    {
        GGCommandStack stk;
        QVERIFY(stk.execute(inc));
        QVERIFY(stk.execute(dbl));
        QVERIFY(stk.execute(new Incer(i)));
        QVERIFY(stk.undo());
        QVERIFY(i == 30);
        grp = stk.toCommandGroup();
    }

    QVERIFY2(grp->state() == GGCommandGroup::Executed, "Command group from stack is not executed");
    QVERIFY2(grp->commands().size() == 2, "Command group has wrong size");
    QVERIFY2(grp->commands()[0] == inc, "Fist command not correct");
    QVERIFY2(grp->commands()[1] == dbl, "Fist command not correct");
    delete grp;
}

void GGEditor_BasicCommandTest::testCommandStackMergeGroup()
{
    int i = 10;
    Incer *inc = new Incer(i);
    Doubler *dbl = new Doubler(i);

    GGCommandStack stk1;
    QVERIFY(stk1.execute(new Incer(i)));
    QVERIFY(stk1.execute(new Doubler(i)));
    QVERIFY(stk1.undo());
    QVERIFY(i == 15);

    {
        GGCommandStack stk2;
        stk2.execute(inc);
        stk2.execute(dbl);
        stk2.execute(new Incer(i));
        stk2.undo();
        QVERIFY(i == 40);

        stk1.mergeAsGroup(stk2);
    }

    QVERIFY2(stk1.getUndoCommands().size() == 2, "Stack size wrong after merging as group");
    QVERIFY2(dynamic_cast<GGCommandGroup*>(stk1.undoCommand()), "Undo command is not a group after merging group");
    GGCommandGroup *grp = dynamic_cast<GGCommandGroup*>(stk1.undoCommand());
    QVERIFY2(grp->commands().size() == 2, "Group size wrong after merging group");
    QVERIFY2(grp->commands()[0] == inc, "First command wrong in group");
    QVERIFY2(grp->commands()[1] == dbl, "second command wrong in group");

    {
        GGCommandStack stk2;
        Incer *inc2 = new Incer(i);
        // For redoability
        stk1.execute(inc2);
        stk1.undo();
        stk1.mergeAsGroup(stk2);
        QVERIFY2(stk1.redoCommand() == inc2, "Redoability gone after merging with empty stack");
    }

    {
        GGCommandStack stk2;
        Incer *inc2 = new Incer(i);
        // For redoability
        stk1.execute(inc2);
        stk1.undo();

        stk2.execute(new Doubler(i));
        stk2.undo();
        stk1.mergeAsGroup(stk2);
        QVERIFY2(stk1.redoCommand() == inc2, "Redoability gone after merging with all undone stack");
    }
}

void GGEditor_BasicCommandTest::testCommandGroup_data()
{
    QTest::addColumn<QString>("failing");
    QTest::newRow("first") << "first";
    QTest::newRow("second") << "second";
}

void GGEditor_BasicCommandTest::testCommandGroup()
{
    QFETCH(QString, failing);

    GGCommandGroup grp;
    QVERIFY2(grp.commands().isEmpty(), "Initial wrong");

    DummyCommand *d1 = new DummyCommand(true, "first");
    grp.addCommand(d1);
    QVERIFY2(grp.commands().size() == 1, "Wrong size after adding command");

    DummyCommand *c = new DummyCommand(true, "tmp");
    c->execute();
    grp.addCommand(c);
    QVERIFY2(grp.commands().size() == 1, "Wrong size after adding executed command");
    delete c;

    DummyCommand *d2 = new DummyCommand(true, "second");
    grp.addCommand(d2);
    QVERIFY(grp.commands().size() == 2);

    if (failing == "first")
        c = d1;
    else if (failing == "second")
        c = d2;
    else
        QFAIL("Wrong data");


    c->ret = false;
    QVERIFY2(!grp.execute(), "Can execute with failing command");
    QVERIFY(grp.commands().size() == 2);
    QVERIFY(grp.state() == GGAbstractCommand::NotExecuted);
    if (c == d1) {
        QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::NotExecuted);
    } else {
        QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::Undone);
    }
    QVERIFY(grp.commands()[1]->state() == GGAbstractCommand::NotExecuted);
    QVERIFY(grp.error() == failing + " failed execute");

    c->ret = true;
    QVERIFY2(grp.execute(), "Cannot execute");
    QVERIFY(grp.commands().size() == 2);
    QVERIFY(grp.state() == GGAbstractCommand::Executed);
    QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::Executed);
    QVERIFY(grp.commands()[1]->state() == GGAbstractCommand::Executed);
    QVERIFY(grp.error().isEmpty());

    DummyCommand cc(true);
    grp.addCommand(&cc);
    QVERIFY2(grp.commands().size() == 2, "Can add to executed group");

    c->ret = false;
    QVERIFY2(!grp.undo(), "Can undo with failed");
    QVERIFY(grp.commands().size() == 2);
    QVERIFY(grp.state() == GGAbstractCommand::Executed);
    QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::Executed);
    QVERIFY(grp.commands()[1]->state() == GGAbstractCommand::Executed);
    QVERIFY(grp.error() == failing + " failed undo");

    c->ret = true;
    QVERIFY2(grp.undo(), "Cannot undo");
    QVERIFY(grp.commands().size() == 2);
    QVERIFY(grp.state() == GGAbstractCommand::Undone);
    QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::Undone);
    QVERIFY(grp.commands()[1]->state() == GGAbstractCommand::Undone);
    QVERIFY(grp.error().isEmpty());

    c->ret = false;
    QVERIFY2(!grp.redo(), "Can redo with failed");
    QVERIFY(grp.commands().size() == 2);
    QVERIFY(grp.state() == GGAbstractCommand::Undone);
    QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::Undone);
    QVERIFY(grp.commands()[1]->state() == GGAbstractCommand::Undone);
    QVERIFY(grp.error() == failing + " failed redo");

    c->ret = true;
    QVERIFY2(grp.redo(), "Cannot redo");
    QVERIFY(grp.commands().size() == 2);
    QVERIFY(grp.state() == GGAbstractCommand::Executed);
    QVERIFY(grp.commands()[0]->state() == GGAbstractCommand::Executed);
    QVERIFY(grp.commands()[1]->state() == GGAbstractCommand::Executed);
    QVERIFY(grp.error().isEmpty());
}
