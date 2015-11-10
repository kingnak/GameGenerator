#ifndef GGEDITOR_BASICOBJECTTEST_H
#define GGEDITOR_BASICOBJECTTEST_H

#include <QtTest>

class GGEditor_BasicObjectTest : public QObject
{
    Q_OBJECT
public:
    GGEditor_BasicObjectTest();

private slots:
    void testPageCast();
    void testConnectionSlot();
};

#endif // GGEDITOR_BASICOBJECTTEST_H
