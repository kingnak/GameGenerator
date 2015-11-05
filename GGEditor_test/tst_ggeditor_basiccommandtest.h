#ifndef TST_GGEDITOR_BASICCOMMANDTEST_H
#define TST_GGEDITOR_BASICCOMMANDTEST_H

#include <QtTest>

class GGEditor_BasicCommandTest : public QObject
{
    Q_OBJECT

public:
    GGEditor_BasicCommandTest() {}

private slots:
    void testCreatePage();
};

#endif // TST_GGEDITOR_BASICCOMMANDTEST_H

