#ifndef TST_GGEDITOR_VIEWCOMMANDTEST_H
#define TST_GGEDITOR_VIEWCOMMANDTEST_H

#include <QtTest>

class GGEditor_ViewCommandTest : public QObject
{
    Q_OBJECT
public:
    explicit GGEditor_ViewCommandTest(QObject *parent = 0);

private slots:
    void testCreateViewPage();
};

#endif // TST_GGEDITOR_VIEWCOMMANDTEST_H
