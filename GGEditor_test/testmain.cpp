#include "tst_ggeditor_basicmodeltest.cpp"
#include "tst_ggeditor_basiccommandtest.cpp"

int main(int argc, char *argv[])
{
    GGEditor_BasicModelTest tcBasicModel;
    GGEditor_BasicCommandTest tcBasicCommand;

    QTest::setMainSourcePath(__FILE__);
    int res = QTest::qExec(&tcBasicModel, argc, argv);
    res |= QTest::qExec(&tcBasicCommand, argc, argv);
    return res;
}

