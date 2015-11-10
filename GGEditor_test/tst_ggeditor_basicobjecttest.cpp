#include "tst_ggeditor_basicobjecttest.h"
#include <model/ggpage.h>
#include <model/ggconnectionslot.h>
#include <model/ggeditmodel.h>
#include <model/ggconnection.h>
#include <model/ggsimplefactory.h>

GGEditor_BasicObjectTest::GGEditor_BasicObjectTest()
{

}

void GGEditor_BasicObjectTest::testPageCast()
{
    GGPage *p = new GGStartPage();
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == p, "Cannot cast Startpage to itself");
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == 0, "Can cast Startpage to EndPage");
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == 0, "Can cast Startpage to ConditionPage");
    QVERIFY2(ggpage_cast<GGActionPage*>(p) == 0, "Can cast Startpage to ActionPage");
    QVERIFY2(ggpage_cast<GGDecisionPage*>(p) == 0, "Can cast Startpage to DecisionPage");

    const GGPage *cp = p;
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == cp, "Cannot cast const Startpage to itself");
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == 0, "Can cast const Startpage to EndPage");
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == 0, "Can cast const Startpage to ConditionPage");
    QVERIFY2(ggpage_cast<const GGActionPage*>(cp) == 0, "Can cast const Startpage to ActionPage");
    QVERIFY2(ggpage_cast<const GGDecisionPage*>(cp) == 0, "Can cast const Startpage to DecisionPage");
    delete p;

    p = new GGEndPage();
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == p, "Cannot cast EndPage to itself");
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == 0, "Can cast EndPage to StartPage");
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == 0, "Can cast EndPage to ConditionPage");
    QVERIFY2(ggpage_cast<GGActionPage*>(p) == 0, "Can cast EndPage to ActionPage");
    QVERIFY2(ggpage_cast<GGDecisionPage*>(p) == 0, "Can cast EndPage to DecisionPage");

    cp = p;
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == cp, "Cannot cast const EndPage to itself");
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == 0, "Can cast const EndPage to StartPage");
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == 0, "Can cast const EndPage to ConditionPage");
    QVERIFY2(ggpage_cast<const GGActionPage*>(cp) == 0, "Can cast const EndPage to ActionPage");
    QVERIFY2(ggpage_cast<const GGDecisionPage*>(cp) == 0, "Can cast const EndPage to DecisionPage");
    delete p;

    p = new GGConditionPage();
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == p, "Cannot cast ConditionPage to itself");
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == 0, "Can cast ConditionPage to EndPage");
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == 0, "Can cast ConditionPage to StartPage");
    QVERIFY2(ggpage_cast<GGActionPage*>(p) == 0, "Can cast ConditionPage to ActionPage");
    QVERIFY2(ggpage_cast<GGDecisionPage*>(p) == 0, "Can cast ConditionPage to DecisionPage");

    cp = p;
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == cp, "Cannot cast const ConditionPage to itself");
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == 0, "Cannot cast const ConditionPage to EndPage");
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == 0, "Can cast const ConditionPage to StartPage");
    QVERIFY2(ggpage_cast<const GGActionPage*>(cp) == 0, "Can cast const ConditionPage to ActionPage");
    QVERIFY2(ggpage_cast<const GGDecisionPage*>(cp) == 0, "Can cast const ConditionPage to DecisionPage");
    delete p;

    p = new GGActionPage();
    QVERIFY2(ggpage_cast<GGActionPage*>(p) == p, "Cannot cast ActionPage to itself");
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == 0, "Can cast ActionPage to StartPage");
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == 0, "Can cast ActionPage to EndPage");
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == 0, "Can cast ActionPage to ConditionPage");
    QVERIFY2(ggpage_cast<GGDecisionPage*>(p) == 0, "Can cast ActionPage to DecisionPage");

    cp = p;
    QVERIFY2(ggpage_cast<const GGActionPage*>(cp) == cp, "Cannot cast const ActionPage to itself");
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == 0, "Can cast const ActionPage to StartPage");
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == 0, "Can cast const ActionPage to EndPage");
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == 0, "Can cast const ActionPage to ConditionPage");
    QVERIFY2(ggpage_cast<const GGDecisionPage*>(cp) == 0, "Can cast const ActionPage to DecisionPage");
    delete p;

    p = new GGDecisionPage();
    QVERIFY2(ggpage_cast<GGDecisionPage*>(p) == p, "Cannot cast DecisionPage to itself");
    QVERIFY2(ggpage_cast<GGStartPage*>(p) == 0, "Can cast DecisionPage to StartPage");
    QVERIFY2(ggpage_cast<GGEndPage*>(p) == 0, "Can cast DecisionPage to EndPage");
    QVERIFY2(ggpage_cast<GGConditionPage*>(p) == 0, "Can cast DecisionPage to ConditionPage");
    QVERIFY2(ggpage_cast<GGActionPage*>(p) == 0, "Can cast DecisionPage to ActionPage");

    cp = p;
    QVERIFY2(ggpage_cast<const GGDecisionPage*>(cp) == cp, "Cannot cast const DecisionPage to itself");
    QVERIFY2(ggpage_cast<const GGStartPage*>(cp) == 0, "Can cast const DecisionPage to StartPage");
    QVERIFY2(ggpage_cast<const GGEndPage*>(cp) == 0, "Can cast const DecisionPage to EndPage");
    QVERIFY2(ggpage_cast<const GGConditionPage*>(cp) == 0, "Can cast const DecisionPage to ConditionPage");
    QVERIFY2(ggpage_cast<const GGActionPage*>(cp) == 0, "Can cast const DecisionPage to ActionPage");
    delete p;
}

void GGEditor_BasicObjectTest::testConnectionSlot()
{
    GGEditModel m(new GGSimpleFactory);
    GGStartPage *s = m.factory()->createStartPage();
    GGEndPage *e = m.factory()->createEndPage();
    m.registerNewPage(s);
    m.registerNewPage(e);

    GGConnection *c1 = m.factory()->createConnection(s->id(), e->id());
    GGConnection *o;

    m.registerNewConnection(c1);
    GGConnectionSlot sl(GGConnectionSlot::StartConnection);
    sl.connect(s, c1, &o);

    QVERIFY2(s->getConnections().value(0) == c1, "Connection not set after connect");
    QVERIFY2(o == NULL, "Got an old connection when there was none");

    GGConnection *c2 = m.factory()->createConnection(s->id(), e->id());
    m.registerNewConnection(c2);
    sl.connect(s, c2, &o);
    QVERIFY2(s->getConnections().value(0) == c2, "Connection not replaced after connect");
    QVERIFY2(o == c1, "Did not get old connection");
}
