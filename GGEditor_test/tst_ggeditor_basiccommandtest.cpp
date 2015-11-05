#include "tst_ggeditor_basiccommandtest.h"

#include <command/ggmodelcommands.h>
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggpage.h>

void GGEditor_BasicCommandTest::testCreatePage()
{
    GGEditModel model(new GGSimpleFactory);
    GGCreatePageCmd cmd(&model, GGCreatePageCmd::StartPage);
    QVERIFY(cmd.execute());
    QVERIFY(cmd.createdPage() != NULL);
    QVERIFY(cmd.createdPage()->model() == &model);
    QVERIFY(model.getPage(cmd.createdPage()->id()) == cmd.createdPage());
}
