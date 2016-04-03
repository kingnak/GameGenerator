#include "testsignalchecker.h"
#include <model/ggabstractmodel.h>
#include <viewmodel/ggviewmodel.h>
#include <QtTest>

ModelSignalChecker::ModelSignalChecker(GGAbstractModel *m, QObject *parent) :
    QObject(parent), m_sr(0), m_su(0), m_pr(0), m_pu(0), m_cr(0), m_cu(0), m_pc(0)
{
    connect(m, SIGNAL(sceneRegistered(GGScene*)), this, SLOT(sceneReg()));
    connect(m, SIGNAL(sceneUnregistered(GG::SceneID,GGScene*)), this, SLOT(sceneUnreg()));
    connect(m, SIGNAL(pageRegistered(GGPage*)), this, SLOT(pageReg()));
    connect(m, SIGNAL(pageUnregistered(GG::PageID, GGPage*)), this, SLOT(pageUnReg()));
    connect(m, SIGNAL(connectionRegistered(GGConnection*)), this, SLOT(connReg()));
    connect(m, SIGNAL(connectionUnregistered(GG::ConnectionID,GGConnection*)), this, SLOT(connUnReg()));
    connect(m, SIGNAL(pageUpdated(GGPage*, GGAbstractModel::PageSections)), this, SLOT(pageCh()));
}

void ModelSignalChecker::verify(QString s, int sr, int su, int pr, int pu, int cr, int cu, int pc, bool r)
{
    VERIFYSIGR(this, s, sr, su, pr, pu, cr, cu, pc, r);
}
/*
#define QVERIFY2(statement, description) \
do {\
    if (statement) {\
        if (!QTest::qVerify(true, #statement, (description), __FILE__, __LINE__))\
            return;\
    } else {\
        if (!QTest::qVerify(false, #statement, (description), __FILE__, __LINE__))\
            return;\
    }\
} while (0)
*/


bool ModelSignalChecker::verifyFL(QString s, QString f, int l, int sr, int su, int pr, int pu, int cr, int cu, int pc, bool r)
{
    bool ok = true;
#define LOCVERIFY(statement, description, file, line) \
    do {\
        if (statement) {\
            QTest::qVerify(true, #statement, (description), file, line);\
        } else { \
            QTest::qVerify(false, #statement, (description), file, line);\
            ok = false; \
        }\
    } while(0)



    int lsr = m_sr, lsu = m_su, lpr = m_pr, lpu = m_pu, lcr = m_cr, lcu = m_cu, lpc = m_pc;
    if (r) reset();
    LOCVERIFY(lsr == sr, qPrintable(QString("%3: Expected %1 scene registrations, got %2").arg(sr).arg(lsr).arg(s)), qPrintable(f), l);
    LOCVERIFY(lsu == su, qPrintable(QString("%3: Expected %1 scene unregistrations, got %2").arg(su).arg(lsu).arg(s)), qPrintable(f), l);
    LOCVERIFY(lpr == pr, qPrintable(QString("%3: Expected %1 page registrations, got %2").arg(pr).arg(lpr).arg(s)), qPrintable(f), l);
    LOCVERIFY(lpu == pu, qPrintable(QString("%3: Expected %1 page unregistrations, got %2").arg(pu).arg(lpu).arg(s)), qPrintable(f), l);
    LOCVERIFY(lcr == cr, qPrintable(QString("%3: Expected %1 connection registrations, got %2").arg(cr).arg(lcr).arg(s)), qPrintable(f), l);
    LOCVERIFY(lcu == cu, qPrintable(QString("%3: Expected %1 connection unregistrations, got %2").arg(cu).arg(lcu).arg(s)), qPrintable(f), l);
    LOCVERIFY(lpc == pc, qPrintable(QString("%3: Expected %1 page changes, got %2").arg(pc).arg(lpc).arg(s)), qPrintable(f), l);
    /*
    QVERIFY2(lpr == pr, qPrintable(QString("%3: Expected %1 page registrations, got %2").arg(pr).arg(lpr).arg(s)));
    QVERIFY2(lpu == pu, qPrintable(QString("%3: Expected %1 page unregistrations, got %2").arg(pu).arg(lpu).arg(s)));
    QVERIFY2(lcr == cr, qPrintable(QString("%3: Expected %1 connection registrations, got %2").arg(cr).arg(lcr).arg(s)));
    QVERIFY2(lcu == cu, qPrintable(QString("%3: Expected %1 connection unregistrations, got %2").arg(cu).arg(lcu).arg(s)));
    QVERIFY2(lpc == pc, qPrintable(QString("%3: Expected %1 page changes, got %2").arg(pc).arg(lpc).arg(s)));
    */
#undef LOCVERIFY
    return ok;
}

void ModelSignalChecker::verifyNull(QString s, bool r)
{
    verify(s, 0,0,0,0,0,0,0, r);
}

void ModelSignalChecker::reset()
{
    m_sr = m_su = m_pr = m_pu = m_cr = m_cu = m_pc = 0;
}

void ModelSignalChecker::sceneReg()
{
    m_sr++;
}

void ModelSignalChecker::sceneUnreg()
{
    m_su++;
}

void ModelSignalChecker::pageReg()
{
    m_pr++;
}

void ModelSignalChecker::pageUnReg()
{
    m_pu++;
}

void ModelSignalChecker::connReg()
{
    m_cr++;
}

void ModelSignalChecker::connUnReg()
{
    m_cu++;
}

void ModelSignalChecker::pageCh()
{
    m_pc++;
}

//////////////////////////////////////////////



ViewModelSignalChecker::ViewModelSignalChecker(GGViewModel *m, QObject *parent) :
    QObject(parent), m_sr(0), m_su(0), m_pr(0), m_pu(0), m_cr(0), m_cu(0), m_pc(0), m_vpc(0)
{
    connect(m, SIGNAL(viewSceneRegistered(GGViewScene*)), this, SLOT(sceneReg()));
    connect(m, SIGNAL(viewSceneUnregistered(GGViewScene*)), this, SLOT(sceneUnreg()));
    connect(m, SIGNAL(viewPageRegistered(GGViewPage*)), this, SLOT(pageReg()));
    connect(m, SIGNAL(viewPageUnregistered(GGViewPage*)), this, SLOT(pageUnReg()));
    connect(m, SIGNAL(viewConnectionRegistered(GGViewConnection*)), this, SLOT(connReg()));
    connect(m, SIGNAL(viewConnectionUnregistered(GGViewConnection*)), this, SLOT(connUnReg()));
    connect(m, SIGNAL(pageUpdated(GGViewPage*)), this, SLOT(pageCh()));
    connect(m, SIGNAL(viewPageUpdated(GGViewPage*)), this, SLOT(viewpageCh()));
}

void ViewModelSignalChecker::verify(QString s, int sr, int su, int pr, int pu, int cr, int cu, int pc, int vpc, bool r)
{
    VERIFYVSIGR(this, s, sr, su, pr, pu, cr, cu, pc, vpc, r);
}

bool ViewModelSignalChecker::verifyFL(QString s, QString f, int l, int sr, int su, int pr, int pu, int cr, int cu, int pc, int vpc, bool r)
{
    bool ok = true;
#define LOCVERIFY(statement, description, file, line) \
    do {\
        if (statement) {\
            QTest::qVerify(true, #statement, (description), file, line);\
        } else { \
            QTest::qVerify(false, #statement, (description), file, line);\
            ok = false; \
        }\
    } while(0)



    int lsr = m_sr, lsu = m_su, lpr = m_pr, lpu = m_pu, lcr = m_cr, lcu = m_cu, lpc = m_pc, lvpc = m_vpc;
    if (r) reset();
    LOCVERIFY(lsr == sr, qPrintable(QString("%3: Expected %1 view scene registrations, got %2").arg(sr).arg(lsr).arg(s)), qPrintable(f), l);
    LOCVERIFY(lsu == su, qPrintable(QString("%3: Expected %1 view scene unregistrations, got %2").arg(su).arg(lsu).arg(s)), qPrintable(f), l);
    LOCVERIFY(lpr == pr, qPrintable(QString("%3: Expected %1 view page registrations, got %2").arg(pr).arg(lpr).arg(s)), qPrintable(f), l);
    LOCVERIFY(lpu == pu, qPrintable(QString("%3: Expected %1 view page unregistrations, got %2").arg(pu).arg(lpu).arg(s)), qPrintable(f), l);
    LOCVERIFY(lcr == cr, qPrintable(QString("%3: Expected %1 view connection registrations, got %2").arg(cr).arg(lcr).arg(s)), qPrintable(f), l);
    LOCVERIFY(lcu == cu, qPrintable(QString("%3: Expected %1 view connection unregistrations, got %2").arg(cu).arg(lcu).arg(s)), qPrintable(f), l);
    LOCVERIFY(lpc == pc, qPrintable(QString("%3: Expected %1 model page changes, got %2").arg(pc).arg(lpc).arg(s)), qPrintable(f), l);
    LOCVERIFY(lvpc == vpc, qPrintable(QString("%3: Expected %1 view page changes, got %2").arg(vpc).arg(lvpc).arg(s)), qPrintable(f), l);
#undef LOCVERIFY
    return ok;
}

void ViewModelSignalChecker::verifyNull(QString s, bool r)
{
    verify(s, 0,0,0,0,0,0,0,0, r);
}

void ViewModelSignalChecker::reset()
{
    m_sr = m_su = m_pr = m_pu = m_cr = m_cu = m_pc = m_vpc = 0;
}

void ViewModelSignalChecker::sceneReg()
{
    m_sr++;
}

void ViewModelSignalChecker::sceneUnreg()
{
    m_su++;
}

void ViewModelSignalChecker::pageReg()
{
    m_pr++;
}

void ViewModelSignalChecker::pageUnReg()
{
    m_pu++;
}

void ViewModelSignalChecker::connReg()
{
    m_cr++;
}

void ViewModelSignalChecker::connUnReg()
{
    m_cu++;
}

void ViewModelSignalChecker::pageCh()
{
    m_pc++;
}

void ViewModelSignalChecker::viewpageCh()
{
    m_vpc++;
}
