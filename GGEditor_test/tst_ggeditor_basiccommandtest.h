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
    void testCreatePage();
    void testDeletePage();
    void testDeletePageWithConnection();
};

#endif // TST_GGEDITOR_BASICCOMMANDTEST_H

