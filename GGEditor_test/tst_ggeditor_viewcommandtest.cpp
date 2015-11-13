#include "tst_ggeditor_viewcommandtest.h"
#include <viewcommand/ggviewcommands.h>
#include <viewcommand/ggviewcommandfactory.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include "testsignalchecker.h"

GGEditor_ViewCommandTest::GGEditor_ViewCommandTest(QObject *parent) : QObject(parent)
{

}

void GGEditor_ViewCommandTest::testCreateViewPage()
{
    GGEditModel em(new GGSimpleFactory);
    GGViewModel vm(&em);
    GGViewCommandFactory vf(&vm);
    GGCreateViewPageCmd *cmd = vf.createStartPage(QRect(0, 0, 1 ,1));
    ViewModelSignalChecker vmsc(&vm);

    QVERIFY2(cmd->execute(), "Cannot execute create view page");
    VERIFYVSIG(&vmsc, "Create View Page CMD", 1, 0, 0, 0, 0, 1);
    GGPage *p = cmd->getInnerCommand()->createdPage();
    QVERIFY2(em.getPages().size() == 1, "No page created");
    QVERIFY2(em.getPage(p->id()) == p, "Create page is not the page from the command");
    QVERIFY2(vm.getViewPageForPage(p) != NULL, "No View Page Created");
    QVERIFY2(vm.getViewPageForPage(p)->page() == p, "View Page not associated with page");
    GGViewPage *vp = vm.getViewPageForPage(p);
    QVERIFY2(!vp->bounds().isEmpty(), "Bounds not set");

    QVERIFY2(cmd->undo(), "Cannot undo create view page");
    VERIFYVSIG(&vmsc, "Undo Create View Page CMD", 0, 1, 0, 0, 0, 0);
    QVERIFY2(em.getPages().isEmpty(), "Page still there after undo create view page");
    //QVERIFY2(vm.getViewPageForPage(p) == vp, "View Page lost before processing commands after undo create view page");
    //qApp->processEvents();
    QVERIFY2(vm.getViewPageForPage(p) == NULL, "View Page still there after undo create view page");

    QVERIFY2(cmd->redo(), "Cannot redo create view page");
    VERIFYVSIG(&vmsc, "Redo Create View Page CMD", 1, 0, 0, 0, 0, 0);
    //QVERIFY2(vm.getViewPageForPage(p) == NULL, "View Page already there before processing commands after redo create view page");
    //qApp->processEvents();
    QVERIFY2(vm.getViewPageForPage(p) != NULL, "View Page lost after processing commands after redo create view page");
    QVERIFY2(vm.getViewPageForPage(p) == vp, "View Page changed (not recycled) after redoing create view page");
    VERIFYVSIGNULL(&vmsc, "After Create");

    delete cmd;
}

