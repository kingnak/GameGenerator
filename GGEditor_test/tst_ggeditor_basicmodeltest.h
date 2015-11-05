#ifndef TST_GGEDITOR_BASICMODELTEST_H
#define TST_GGEDITOR_BASICMODELTEST_H

#include <QString>
#include <QtTest>
#include <model/ggsimplefactory.h>
#include <model/ggeditmodel.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>

class GGEditor_BasicModelTest : public QObject
{
    Q_OBJECT

public:
    GGEditor_BasicModelTest();

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void testRegister();
    void testUnregisterStart();
    void testUnregisterEnd();
    void testUnregisterConnection();
    void testReregister();

private:
    void setupModel(GGPage *&s, GGPage *&e, GGConnection *&c);

private:
    GGEditModel *model;
    GGPage *s, *e;
    GGConnection *c;
    bool m_warnedAboutConnectionHack;
};

#endif // TST_GGEDITOR_BASICMODELTEST_H

