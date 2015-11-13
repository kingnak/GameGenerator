#include "tst_ggeditor_basicmodeltest.cpp"
#include "tst_ggeditor_basiccommandtest.cpp"
#include "tst_ggeditor_basicobjecttest.cpp"
#include "tst_ggeditor_connectionslottest.cpp"
#include "tst_ggeditor_modelcommandtest.cpp"
#include "tst_ggeditor_viewcommandtest.cpp"
#ifdef USE_VLD
#include <vld.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    GGEditor_BasicObjectTest tcBasicObject;
    GGEditor_BasicModelTest tcBasicModel;
    GGEditor_BasicCommandTest tcBasicCommand;
    GGEditor_ModelCommandTest tcModelCommand;
    GGEditor_ConnectionSlotTest tcConnectionSlot;
    GGEditor_ViewCommandTest tcViewCommand;

    //QTest::setMainSourcePath(__FILE__);
    int res = QTest::qExec(&tcBasicObject, argc, argv);
    res |= QTest::qExec(&tcBasicModel, argc, argv);
    res |= QTest::qExec(&tcBasicCommand, argc, argv);
    res |= QTest::qExec(&tcModelCommand, argc, argv);
    res |= QTest::qExec(&tcConnectionSlot, argc, argv);
    res |= QTest::qExec(&tcViewCommand, argc, argv);
    return res;
}

