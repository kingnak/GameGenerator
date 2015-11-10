#ifndef TST_GGEDITOR_CONNECTIONSLOTTEST_H
#define TST_GGEDITOR_CONNECTIONSLOTTEST_H

#include <QtTest>

class GGEditModel;
class GGEditCommandFactory;
class GGCommandStack;
class GGStartPage;
class GGEndPage;
class GGConditionPage;
class GGActionPage;
class GGDecisionPage;

class GGEditor_ConnectionSlotTest : public QObject
{
    Q_OBJECT
public:
    GGEditor_ConnectionSlotTest() {}

private slots:
    void init();
    void cleanup();

    void testConnectStart();
    void testConnectEnd();
    void testConnectCondition();
    void testActionConnection();
    void testMappedConnection_data();
    void testMappedConnection();

private:
    GGEditModel *m_model;
    GGEditCommandFactory *m_cmdFac;
    GGCommandStack *m_stack;
    GGStartPage *m_s;
    GGEndPage *m_e;
    GGConditionPage *m_c;
    GGActionPage *m_a;
    GGDecisionPage *m_d;
};

#endif // TST_GGEDITOR_CONNECTIONSLOTTEST_H
