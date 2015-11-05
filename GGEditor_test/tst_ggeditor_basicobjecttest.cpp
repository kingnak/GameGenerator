#include "tst_ggeditor_basicobjecttest.h"
#include <model/ggpage.h>

GGEditor_BasicObjectTest::GGEditor_BasicObjectTest()
{

}

void GGEditor_BasicObjectTest::testPageCast()
{
    GGPage *p = new GGStartPage();
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == p, "Cannot cast Startpage to itself");
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == 0, "Can cast Startpage to EndPage");
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == 0, "Can cast Startpage to ConditionPage");

    const GGPage *cp = p;
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == cp, "Cannot cast const Startpage to itself");
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == 0, "Can cast const Startpage to EndPage");
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == 0, "Can cast const Startpage to ConditionPage");
    delete p;

    p = new GGEndPage();
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == p, "Cannot cast EndPage to itself");
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == 0, "Can cast EndPage to StartPage");
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == 0, "Can cast EndPage to ConditionPage");

    cp = p;
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == cp, "Cannot cast const EndPage to itself");
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == 0, "Can cast const EndPage to StartPage");
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == 0, "Can cast const EndPage to ConditionPage");

    delete p;

    p = new GGConditionPage();
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == p, "Cannot cast ConditionPage to itself");
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == 0, "Can cast ConditionPage to EndPage");
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == 0, "Can cast ConditionPage to StartPage");

    cp = p;
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == cp, "Can cast const ConditionPage to itself");
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == 0, "Cannot cast const ConditionPage to EndPage");
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == 0, "Can cast const ConditionPage to StartPage");
}

