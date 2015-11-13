#ifndef TST_GGEDITOR_MODELCOMMANDTEST_H
#define TST_GGEDITOR_MODELCOMMANDTEST_H

#include <QtTest>

class GGEditModel;
class GGEditCommandFactory;
class GGCommandStack;
class ModelSignalChecker;

class GGEditor_ModelCommandTest : public QObject
{
    Q_OBJECT
public:
    explicit GGEditor_ModelCommandTest(QObject *parent = 0);

private slots:
    void init();
    void cleanup();

    void testCreatePage();
    void testDeletePage();
    void testCreateConnection();
    void testDeleteConnection();

    void testDeletePageWithConnection();

    void testSimpleSetterCommands();
    void testMappedLinkCommands();
    void testDecisionLinkCommands();

private:
    GGEditModel *m_model;
    GGEditCommandFactory *m_fac;
    GGCommandStack *m_stk;
    ModelSignalChecker *m_sc;
};

#endif // TST_GGEDITOR_MODELCOMMANDTEST_H
