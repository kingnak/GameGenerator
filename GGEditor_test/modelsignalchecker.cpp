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
    QVERIFY2(m_pr == pr, qPrintable(QString("%3: Expected %1 page registrations, got %2").arg(pr).arg(m_pr).arg(s)));
    QVERIFY2(m_pu == pu, qPrintable(QString("%3: Expected %1 page unregistrations, got %2").arg(pu).arg(m_pu).arg(s)));
    QVERIFY2(m_cr == cr, qPrintable(QString("%3: Expected %1 connection registrations, got %2").arg(cr).arg(m_cr).arg(s)));
    QVERIFY2(m_cu == cu, qPrintable(QString("%3: Expected %1 connection unregistrations, got %2").arg(cu).arg(m_cu).arg(s)));
    QVERIFY2(m_pc == pc, qPrintable(QString("%3: Expected %1 page changes, got %2").arg(pc).arg(m_pc).arg(s)));
    if (r) reset();
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

