#ifndef TST_GGEDITOR_BASICCOMMANDTEST_H
#define TST_GGEDITOR_BASICCOMMANDTEST_H

#include <QtTest>

class GGEditor_BasicCommandTest : public QObject
{
    Q_OBJECT

public:
    GGEditor_BasicCommandTest() {}

private slots:
    void testCommandBasics();
    void testCommandGroup_data();
    void testCommandGroup();
    void testCommandStack();
    void testCommandStackWithFails();

    void testCommandGroupOrder();
    void testCommandStackOrder();

    void testCommandStackMerge();
    void testCommandGroupFromStack();
    void testCommandStackMergeGroup();

};

#endif // TST_GGEDITOR_BASICCOMMANDTEST_H

