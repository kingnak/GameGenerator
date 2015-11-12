#include "modelsignalchecker.h"
#include <model/ggabstractmodel.h>
#include <QtTest>

ModelSignalChecker::ModelSignalChecker(GGAbstractModel *m, QObject *parent) :
    QObject(parent), m_pr(0), m_pu(0), m_cr(0), m_cu(0), m_pc(0)
{
    connect(m, SIGNAL(pageRegistered(GGPage*)), this, SLOT(pageReg()));
    connect(m, SIGNAL(pageUnregistered(GG::PageID, GGPage*)), this, SLOT(pageUnReg()));
    connect(m, SIGNAL(connectionRegistered(GGConnection*)), this, SLOT(connReg()));
    connect(m, SIGNAL(connectionUnRegistered(GG::ConnectionID,GGConnection*)), this, SLOT(connUnReg()));
    connect(m, SIGNAL(pageUpdated(GGPage*)), this, SLOT(pageCh()));
}

void ModelSignalChecker::verify(QString s, int pr, int pu, int cr, int cu, int pc, bool r)
{
    VERIFYSIGR(this, s, pr, pu, cr, cu, pc, r);
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


bool ModelSignalChecker::verifyFL(QString s, QString f, int l, int pr, int pu, int cr, int cu, int pc, bool r)
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



    int lpr = m_pr, lpu = m_pu, lcr = m_cr, lcu = m_cu, lpc = m_pc;
    if (r) reset();
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
    verify(s, 0,0,0,0,0, r);
}

void ModelSignalChecker::reset()
{
    m_pr = m_pu = m_cr = m_cu = m_pc = 0;
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

