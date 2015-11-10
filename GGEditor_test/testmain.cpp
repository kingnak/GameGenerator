#include "tst_ggeditor_basicmodeltest.cpp"
#include "tst_ggeditor_basiccommandtest.cpp"
#include "tst_ggeditor_basicobjecttest.cpp"
#include "tst_ggeditor_connectionslottest.cpp"

#ifdef USE_VLD
#include <vld.h>
#endif

int main(int argc, char *argv[])
{
    GGEditor_BasicObjectTest tcBasicObject;
    GGEditor_BasicModelTest tcBasicModel;
    GGEditor_BasicCommandTest tcBasicCommand;
    GGEditor_ConnectionSlotTest tcConnectionSlot;

    //QTest::setMainSourcePath(__FILE__);
    int res = QTest::qExec(&tcBasicObject, argc, argv);
    res |= QTest::qExec(&tcBasicModel, argc, argv);
    res |= QTest::qExec(&tcBasicCommand, argc, argv);
    res |= QTest::qExec(&tcConnectionSlot, argc, argv);
    return res;
}

