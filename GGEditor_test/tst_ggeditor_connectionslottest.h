#ifndef TST_GGEDITOR_CONNECTIONSLOTTEST_H
#define TST_GGEDITOR_CONNECTIONSLOTTEST_H

#include <QtTest>

class GGEditModel;
class GGEditCommandFactory;
class GGCommandStack;

class GGEditor_ConnectionSlotTest : public QObject
{
    Q_OBJECT
public:
    GGEditor_ConnectionSlotTest() {}

private slots:
    void init();
    void cleanup();

    void testConnectionSlots();

private:
    GGEditModel *m_model;
    GGEditCommandFactory *m_cmdFac;
    //GGCommandStack *m_stack;
};

#endif // TST_GGEDITOR_CONNECTIONSLOTTEST_H
