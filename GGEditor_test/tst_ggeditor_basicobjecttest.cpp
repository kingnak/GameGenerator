#include "tst_ggeditor_basicobjecttest.h"
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnectionslot.h>
#include <model/ggeditmodel.h>
#include <model/ggconnection.h>
#include <model/ggsimplefactory.h>
#include <model/ggmediaresolver.h>
#include "testsignalchecker.h"

GGEditor_BasicObjectTest::GGEditor_BasicObjectTest()
{

}

void GGEditor_BasicObjectTest::testPageCast()
{
    GGPage *p = new GGStartPage(GG::InvalidSceneId);
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

    p = new GGEndPage(GG::InvalidSceneId);
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

    p = new GGConditionPage(GG::InvalidSceneId);
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

    p = new GGActionPage(GG::InvalidSceneId);
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

    p = new GGDecisionPage(GG::InvalidSceneId);
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

void GGEditor_BasicObjectTest::testPageAs()
{
    GGPage *p = new GGStartPage(GG::InvalidSceneId);
    const GGPage *cp = p;
    QVERIFY2(GG::as<GGConditionPage>(p) == 0, "StartPage can act as ConditionPage");
    QVERIFY2(GG::as<GGContentPage>(p) == p, "StartPage cannot act as ContentPage");
    QVERIFY2(GG::as<GGEntryActionPage>(p) == 0, "StartPage can act as EntryActionPage");
    QVERIFY2(GG::as<GGEndPage>(p) == 0, "StartPage can act as EndPage");
    QVERIFY2(GG::as<GGMappedContentPage>(p) == 0, "StartPage can act as MappedContentPage");
    QVERIFY2(GG::as<GGActionPage>(p) == 0, "StartPage can act as ActionPage");
    QVERIFY2(GG::as<GGDecisionPage>(p) == 0, "StartPage can act as DecisionPage");
    QVERIFY2(GG::as<GGStartPage>(p) == p, "StartPage cannot act as StartPage");

    QVERIFY2(GG::as<const GGConditionPage>(cp) == 0, "StartPage can act as Const ConditionPage");
    QVERIFY2(GG::as<const GGContentPage>(cp) == p, "StartPage cannot act as Const ContentPage");
    QVERIFY2(GG::as<const GGEntryActionPage>(cp) == 0, "StartPage can act as Const EntryActionPage");
    QVERIFY2(GG::as<const GGEndPage>(cp) == 0, "StartPage can act as Const EndPage");
    QVERIFY2(GG::as<const GGMappedContentPage>(cp) == 0, "StartPage can act as Const MappedContentPage");
    QVERIFY2(GG::as<const GGActionPage>(cp) == 0, "StartPage can act as Const ActionPage");
    QVERIFY2(GG::as<const GGDecisionPage>(cp) == 0, "StartPage can act as Const DecisionPage");
    QVERIFY2(GG::as<const GGStartPage>(cp) == cp, "StartPage cannot act as Const StartPage");
    delete p;

    p = new GGEndPage(GG::InvalidSceneId);
    cp = p;
    QVERIFY2(GG::as<GGConditionPage>(p) == 0, "EndPage can act as ConditionPage");
    QVERIFY2(GG::as<GGContentPage>(p) == p, "EndPage cannot act as ContentPage");
    QVERIFY2(GG::as<GGEntryActionPage>(p) == p, "EndPage cannot act as EntryActionPage");
    QVERIFY2(GG::as<GGEndPage>(p) == p, "EndPage cannot act as EndPage");
    QVERIFY2(GG::as<GGMappedContentPage>(p) == 0, "EndPage can act as MappedContentPage");
    QVERIFY2(GG::as<GGActionPage>(p) == 0, "EndPage can act as ActionPage");
    QVERIFY2(GG::as<GGDecisionPage>(p) == 0, "EndPage can act as DecisionPage");
    QVERIFY2(GG::as<GGStartPage>(p) == 0, "EndPage can act as StartPage");

    QVERIFY2(GG::as<const GGConditionPage>(cp) == 0, "EndPage can act as Const ConditionPage");
    QVERIFY2(GG::as<const GGContentPage>(cp) == cp, "EndPage cannot act as Const ContentPage");
    QVERIFY2(GG::as<const GGEntryActionPage>(cp) == cp, "EndPage cannot act as Const EntryActionPage");
    QVERIFY2(GG::as<const GGEndPage>(cp) == cp, "EndPage cannot act as Const EndPage");
    QVERIFY2(GG::as<const GGMappedContentPage>(cp) == 0, "EndPage can act as Const MappedContentPage");
    QVERIFY2(GG::as<const GGActionPage>(cp) == 0, "EndPage can act as Const ActionPage");
    QVERIFY2(GG::as<const GGDecisionPage>(cp) == 0, "EndPage can act as Const DecisionPage");
    QVERIFY2(GG::as<const GGStartPage>(cp) == 0, "EndPage can act as Const StartPage");
    delete p;

    p = new GGConditionPage(GG::InvalidSceneId);
    cp = p;
    QVERIFY2(GG::as<GGConditionPage>(p) == p, "ConditionPage cannot act as ConditionPage");
    QVERIFY2(GG::as<GGContentPage>(p) == 0, "ConditionPage can act as ContentPage");
    QVERIFY2(GG::as<GGEntryActionPage>(p) == 0, "ConditionPage can act as EntryActionPage");
    QVERIFY2(GG::as<GGEndPage>(p) == 0, "ConditionPage can act as EndPage");
    QVERIFY2(GG::as<GGMappedContentPage>(p) == 0, "ConditionPage can act as MappedContentPage");
    QVERIFY2(GG::as<GGActionPage>(p) == 0, "ConditionPage can act as ActionPage");
    QVERIFY2(GG::as<GGDecisionPage>(p) == 0, "ConditionPage can act as DecisionPage");
    QVERIFY2(GG::as<GGStartPage>(p) == 0, "ConditionPage can act as StartPage");

    QVERIFY2(GG::as<const GGConditionPage>(cp) == cp, "ConditionPage cannot act as Const ConditionPage");
    QVERIFY2(GG::as<const GGContentPage>(cp) == 0, "ConditionPage can act as Const ContentPage");
    QVERIFY2(GG::as<const GGEntryActionPage>(cp) == 0, "ConditionPage can act as Const EntryActionPage");
    QVERIFY2(GG::as<const GGEndPage>(cp) == 0, "ConditionPage can act as Const EndPage");
    QVERIFY2(GG::as<const GGMappedContentPage>(cp) == 0, "ConditionPage can act as Const MappedContentPage");
    QVERIFY2(GG::as<const GGActionPage>(cp) == 0, "ConditionPage can act as Const ActionPage");
    QVERIFY2(GG::as<const GGDecisionPage>(cp) == 0, "ConditionPage can act as Const DecisionPage");
    QVERIFY2(GG::as<const GGStartPage>(cp) == 0, "ConditionPage can act as Const StartPage");
    delete p;

    p = new GGActionPage(GG::InvalidSceneId);
    cp = p;
    QVERIFY2(GG::as<GGConditionPage>(p) == 0, "ActionPage can act as ConditionPage");
    QVERIFY2(GG::as<GGContentPage>(p) == p, "ActionPage cannot act as ContentPage");
    QVERIFY2(GG::as<GGEntryActionPage>(p) == p, "ActionPage cannot act as EntryActionPage");
    QVERIFY2(GG::as<GGEndPage>(p) == 0, "ActionPage can act as EndPage");
    QVERIFY2(GG::as<GGMappedContentPage>(p) == p, "ActionPage cannot act as MappedContentPage");
    QVERIFY2(GG::as<GGActionPage>(p) == p, "ActionPage cannot act as ActionPage");
    QVERIFY2(GG::as<GGDecisionPage>(p) == 0, "ActionPage can act as DecisionPage");
    QVERIFY2(GG::as<GGStartPage>(p) == 0, "ActionPage can act as StartPage");

    QVERIFY2(GG::as<const GGConditionPage>(cp) == 0, "ActionPage can act as Const ConditionPage");
    QVERIFY2(GG::as<const GGContentPage>(cp) == cp, "ActionPage cannot act as Const ContentPage");
    QVERIFY2(GG::as<const GGEntryActionPage>(cp) == cp, "ActionPage cannot act as Const EntryActionPage");
    QVERIFY2(GG::as<const GGEndPage>(cp) == 0, "ActionPage can act as Const EndPage");
    QVERIFY2(GG::as<const GGMappedContentPage>(cp) == cp, "ActionPage cannot act as Const MappedContentPage");
    QVERIFY2(GG::as<const GGActionPage>(cp) == cp, "ActionPage cannot act as Const ActionPage");
    QVERIFY2(GG::as<const GGDecisionPage>(cp) == 0, "ActionPage can act as Const DecisionPage");
    QVERIFY2(GG::as<const GGStartPage>(cp) == 0, "ActionPage can act as Const StartPage");
    delete p;

    p = new GGDecisionPage(GG::InvalidSceneId);
    cp = p;
    QVERIFY2(GG::as<GGConditionPage>(p) == 0, "DecisionPage can act as ConditionPage");
    QVERIFY2(GG::as<GGContentPage>(p) == p, "DecisionPage cannot act as ContentPage");
    QVERIFY2(GG::as<GGEntryActionPage>(p) == p, "DecisionPage cannot act as EntryActionPage");
    QVERIFY2(GG::as<GGEndPage>(p) == 0, "DecisionPage can act as EndPage");
    QVERIFY2(GG::as<GGMappedContentPage>(p) == p, "DecisionPage cannot act as MappedContentPage");
    QVERIFY2(GG::as<GGActionPage>(p) == 0, "DecisionPage can act as ActionPage");
    QVERIFY2(GG::as<GGDecisionPage>(p) == p, "DecisionPage cannot act as DecisionPage");
    QVERIFY2(GG::as<GGStartPage>(p) == 0, "DecisionPage can act as StartPage");

    QVERIFY2(GG::as<const GGConditionPage>(cp) == 0, "DecisionPage can act as Const ConditionPage");
    QVERIFY2(GG::as<const GGContentPage>(cp) == cp, "DecisionPage cannot act as Const ContentPage");
    QVERIFY2(GG::as<const GGEntryActionPage>(cp) == cp, "DecisionPage cannot act as Const EntryActionPage");
    QVERIFY2(GG::as<const GGEndPage>(cp) == 0, "DecisionPage can act as Const EndPage");
    QVERIFY2(GG::as<const GGMappedContentPage>(cp) == cp, "DecisionPage cannot act as Const MappedContentPage");
    QVERIFY2(GG::as<const GGActionPage>(cp) == 0, "DecisionPage can act as Const ActionPage");
    QVERIFY2(GG::as<const GGDecisionPage>(cp) == cp, "DecisionPage cannot act as Const DecisionPage");
    QVERIFY2(GG::as<const GGStartPage>(cp) == 0, "DecisionPage can act as Const StartPage");
    delete p;
}

void GGEditor_BasicObjectTest::testRegister()
{
    GGEditModel m(new GGSimpleFactory);

    GGStartPage *s1 = m.factory()->createStartPage(GG::InvalidSceneId);
    QVERIFY2(!m.registerNewPage(s1), "Can register page with invalid scene");
    QVERIFY(m.getPages().isEmpty());


    GGScene *sc = m.factory()->createScene();
    QVERIFY2(m.registerNewScene(sc), "Cannot register scene");
    QVERIFY2(m.getScenes()[0] == sc, "Scene not in model");
    QVERIFY2(m.unregisterScene(sc->id()), "Cannot unregister empty scene");
    QVERIFY(m.getScenes().isEmpty());
    QVERIFY2(m.registerSceneWithId(sc), "Cannot re-register scene");
    QVERIFY(m.getScenes()[0] == sc);

    QVERIFY2(s1->setSceneId(sc->id()), "Cannot set Scene Id on page that is not registered");
    QVERIFY(s1->sceneId() == sc->id());
    QVERIFY2(s1->scene() == NULL, "Not registered page has scene after setting scene Id");

    GGStartPage *s = m.factory()->createStartPage(sc->id());
    QVERIFY2(m.registerNewPage(s), "Cannot register page");
    QVERIFY2(sc->pages().contains(s), "Scene does not contain page");
    QVERIFY(m.getPages()[0] == s);
    QVERIFY2(m.unregisterPage(s->id()) == s, "Cannot unregister page");
    QVERIFY2(sc->pages().isEmpty(), "Scene is not empty");
    QVERIFY(m.getPages().isEmpty());
    QVERIFY2(m.registerPageWithId(s), "Cannot re-register page");
    QVERIFY2(sc->pages().contains(s), "Scene does not contain page after re-register");
    QVERIFY(m.getPages()[0] == s);

    QVERIFY2(!s->setSceneId(GG::InvalidSceneId), "Can set scene id on registered page");
    QVERIFY(s->scene() == sc);
    QVERIFY(s->sceneId() == sc->id());

    QVERIFY2(!m.unregisterScene(sc->id()), "Can unregister non-empty scene");
    m.unregisterPage(s->id());
    QVERIFY2(m.unregisterScene(sc->id()) == sc, "Cannot unretister emptied scene");

    QVERIFY2(!m.registerPageWithId(s), "Can register page to removed scene");
}

void GGEditor_BasicObjectTest::testConnectionSlot()
{
    GGEditModel m(new GGSimpleFactory);
    GGScene *sc = m.factory()->createScene();
    m.registerNewScene(sc);
    GGStartPage *s = m.factory()->createStartPage(sc->id());
    GGEndPage *e = m.factory()->createEndPage(sc->id());
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

void GGEditor_BasicObjectTest::testConnectionSlotCanConnect()
{
    GGEditModel m(new GGSimpleFactory);
    GGScene *scn = m.factory()->createScene();
    m.registerNewScene(scn);
    GGStartPage *s = m.factory()->createStartPage(scn->id());
    GGEndPage *e = m.factory()->createEndPage(scn->id());
    GGConditionPage *c = m.factory()->createConditionPage(scn->id());
    GGActionPage *a = m.factory()->createActionPage(scn->id());
    GGDecisionPage *d = m.factory()->createDecisionPage(scn->id());
    GGPage *dest = m.factory()->createEndPage(scn->id());
    m.registerNewPage(s);
    m.registerNewPage(e);
    m.registerNewPage(c);
    m.registerNewPage(a);
    m.registerNewPage(d);
    m.registerNewPage(dest);
    a->addMappedLink(GGMappedLink::rectangle(QRect()));
    d->addMappedLink(GGMappedLink::rectangle(QRect()));
    d->addDecisionLink(GGLink());

    GGConnection *conn = m.factory()->createConnection(s->id(), dest->id());
    // This is hacky:
    s->setStartConnection(conn);
    c->setTrueConnection(conn);
    c->setFalseConnection(conn);
    GGLink l;
    l.setConnection(conn);
    a->setActionLink(l);
    GGMappedLink ml = GGMappedLink::rectangle(QRect());
    ml.setLink(l);
    a->setMappedLink(0, ml);
    d->setDecisionLink(0, l);
    d->setMappedLink(0, ml);

    GGConnectionSlot sN(GGConnectionSlot::NoConnection);
    GGConnectionSlot sS(GGConnectionSlot::StartConnection);
    GGConnectionSlot sT(GGConnectionSlot::TrueConnection);
    GGConnectionSlot sF(GGConnectionSlot::FalseConnection);
    GGConnectionSlot sA(GGConnectionSlot::ActionConnection);
    GGConnectionSlot sM(GGConnectionSlot::MappedConnection, 0);
    GGConnectionSlot sD(GGConnectionSlot::DecisionConnection, 0);

    ModelSignalChecker sc(&m);

    QVERIFY2(!sN.canConnect(s), "Can connect None to Start");
    VERIFYSIGNULL(&sc, "Check Connect None to Start");
    QVERIFY2(!sN.canConnect(e), "Can connect None to End");
    VERIFYSIGNULL(&sc, "Check Connect None to End");
    QVERIFY2(!sN.canConnect(c), "Can connect None to Condition");
    VERIFYSIGNULL(&sc, "Check Connect None to Condition");
    QVERIFY2(!sN.canConnect(a), "Can connect None to Action");
    VERIFYSIGNULL(&sc, "Check Connect None to Action");
    QVERIFY2(!sN.canConnect(d), "Can connect None to Decision");
    VERIFYSIGNULL(&sc, "Check Connect None to Decision");

    QVERIFY2(sS.canConnect(s), "Cannot connect Start to Start");
    VERIFYSIGNULL(&sc, "Check Connect Start to Start");
    QVERIFY2(!sS.canConnect(e), "Can connect Start to End");
    VERIFYSIGNULL(&sc, "Check Connect Start to End");
    QVERIFY2(!sS.canConnect(c), "Can connect Start to Condition");
    VERIFYSIGNULL(&sc, "Check Connect Start to Condition");
    QVERIFY2(!sS.canConnect(a), "Can connect Start to Action");
    VERIFYSIGNULL(&sc, "Check Connect Start to Action");
    QVERIFY2(!sS.canConnect(d), "Can connect Start to Decision");
    VERIFYSIGNULL(&sc, "Check Connect Start to Decision");

    QVERIFY2(!sT.canConnect(s), "Can connect True to Start");
    VERIFYSIGNULL(&sc, "Check Connect True to Start");
    QVERIFY2(!sT.canConnect(e), "Can connect True to End");
    VERIFYSIGNULL(&sc, "Check Connect True to End");
    QVERIFY2(sT.canConnect(c), "Cannot connect True to Condition");
    VERIFYSIGNULL(&sc, "Check Connect True to Condition");
    QVERIFY2(!sT.canConnect(a), "Can connect True to Action");
    VERIFYSIGNULL(&sc, "Check Connect True to Action");
    QVERIFY2(!sT.canConnect(d), "Can connect True to Decision");
    VERIFYSIGNULL(&sc, "Check Connect True to Decision");

    QVERIFY2(!sF.canConnect(s), "Can connect False to Start");
    VERIFYSIGNULL(&sc, "Check Connect False to Start");
    QVERIFY2(!sF.canConnect(e), "Can connect False to End");
    VERIFYSIGNULL(&sc, "Check Connect False to End");
    QVERIFY2(sF.canConnect(c), "Cannot connect False to Condition");
    VERIFYSIGNULL(&sc, "Check Connect False to Condition");
    QVERIFY2(!sF.canConnect(a), "Can connect False to Action");
    VERIFYSIGNULL(&sc, "Check Connect False to Action");
    QVERIFY2(!sF.canConnect(d), "Can connect False to Decision");
    VERIFYSIGNULL(&sc, "Check Connect False to Decision");

    QVERIFY2(!sA.canConnect(s), "Can connect Action to Start");
    VERIFYSIGNULL(&sc, "Check Connect Action to Start");
    QVERIFY2(!sA.canConnect(e), "Can connect Action to End");
    VERIFYSIGNULL(&sc, "Check Connect Action to End");
    QVERIFY2(!sA.canConnect(c), "Can connect Action to Condition");
    VERIFYSIGNULL(&sc, "Check Connect Action to Condition");
    QVERIFY2(sA.canConnect(a), "Cannot connect Action to Action");
    VERIFYSIGNULL(&sc, "Check Connect Action to Action");
    QVERIFY2(!sA.canConnect(d), "Can connect Action to Decision");
    VERIFYSIGNULL(&sc, "Check Connect Action to Decision");

    QVERIFY2(!sM.canConnect(s), "Can connect Mapped to Start");
    VERIFYSIGNULL(&sc, "Check Connect Mapped to Start");
    QVERIFY2(!sM.canConnect(e), "Can connect Mapped to End");
    VERIFYSIGNULL(&sc, "Check Connect Mapped to End");
    QVERIFY2(!sM.canConnect(c), "Can connect Mapped to Condition");
    VERIFYSIGNULL(&sc, "Check Connect Mapped to Condition");
    QVERIFY2(sM.canConnect(a), "Cannot connect Mapped to Action");
    VERIFYSIGNULL(&sc, "Check Connect Mapped to Action");
    QVERIFY2(sM.canConnect(d), "Cannot connect Mapped to Decision");
    VERIFYSIGNULL(&sc, "Check Connect Mapped to Decision");

    QVERIFY2(!sD.canConnect(s), "Can connect Decision to Start");
    VERIFYSIGNULL(&sc, "Check Connect Decision to Start");
    QVERIFY2(!sD.canConnect(e), "Can connect Decision to End");
    VERIFYSIGNULL(&sc, "Check Connect Decision to End");
    QVERIFY2(!sD.canConnect(c), "Can connect Decision to Condition");
    VERIFYSIGNULL(&sc, "Check Connect Decision to Condition");
    QVERIFY2(!sD.canConnect(a), "Can connect Decision to Action");
    VERIFYSIGNULL(&sc, "Check Connect Decision to Action");
    QVERIFY2(sD.canConnect(d), "Can connect Decision to Decision");
    VERIFYSIGNULL(&sc, "Check Connect Decision to Decision");

    delete conn;
}
