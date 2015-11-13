#ifndef TESTSIGNALCHECKER_H
#define TESTSIGNALCHECKER_H

#include <QObject>

class GGAbstractModel;
class GGViewModel;

class ModelSignalChecker : public QObject
{
    Q_OBJECT
public:
    explicit ModelSignalChecker(GGAbstractModel *m, QObject *parent = 0);

    void verify(QString s, int pr, int pu, int cr, int cu, int pc, bool r = true);
    bool verifyFL(QString s, QString f, int l, int pr, int pu, int cr, int cu, int pc, bool r = true);
    void verifyNull(QString s, bool r = true);
    void reset();

private slots:
    void pageReg();
    void pageUnReg();
    void connReg();
    void connUnReg();
    void pageCh();

private:
    int m_pr, m_pu, m_cr, m_cu, m_pc;
};

#define VERIFYSIGR_STOP(sc, statement, PR, PU, CR, CU, PC, R) \
do {\
    if (!(sc)->verifyFL(statement, __FILE__, __LINE__, PR, PU, CR, CU, PC, R)) {\
        return;\
    }\
} while (0)

#define VERIFYSIGR(sc, statement, PR, PU, CR, CU, PC, R) \
    (sc)->verifyFL(statement, __FILE__, __LINE__, PR, PU, CR, CU, PC, R)

#define VERIFYSIG(sc, statement, PR, PU, CR, CU, PC) VERIFYSIGR(sc, statement, PR, PU, CR, CU, PC, true)
#define VERIFYSIG_STOP(sc, statement, PR, PU, CR, CU, PC) VERIFYSIGR_STOP(sc, statement, PR, PU, CR, CU, PC, true)

#define VERIFYSIGNULL(sc, statement) VERIFYSIGR(sc, statement, 0, 0, 0, 0, 0, true)

///////////////////////////////

class ViewModelSignalChecker : public QObject
{
    Q_OBJECT
public:
    explicit ViewModelSignalChecker(GGViewModel *m, QObject *parent = 0);

    void verify(QString s, int pr, int pu, int cr, int cu, int pc, int vpc, bool r = true);
    bool verifyFL(QString s, QString f, int l, int pr, int pu, int cr, int cu, int pc, int vpc, bool r = true);
    void verifyNull(QString s, bool r = true);
    void reset();

private slots:
    void pageReg();
    void pageUnReg();
    void connReg();
    void connUnReg();
    void pageCh();
    void viewpageCh();

private:
    int m_pr, m_pu, m_cr, m_cu, m_pc, m_vpc;
};

#define VERIFYVSIGR_STOP(sc, statement, PR, PU, CR, CU, PC, VPC, R) \
do {\
    if (!(sc)->verifyFL(statement, __FILE__, __LINE__, PR, PU, CR, CU, PC, VPC, R)) {\
        return;\
    }\
} while (0)

#define VERIFYVSIGR(sc, statement, PR, PU, CR, CU, PC, VPC, R) \
    (sc)->verifyFL(statement, __FILE__, __LINE__, PR, PU, CR, CU, PC, VPC, R)

#define VERIFYVSIG(sc, statement, PR, PU, CR, CU, PC, VPC) VERIFYVSIGR(sc, statement, PR, PU, CR, CU, PC, VPC, true)
#define VERIFYVSIG_STOP(sc, statement, PR, PU, CR, CU, PC, VPC) VERIFYVSIGR_STOP(sc, statement, PR, PU, CR, CU, PC, VPC, true)

#define VERIFYVSIGNULL(sc, statement) VERIFYVSIGR(sc, statement, 0, 0, 0, 0, 0, 0, true)

#endif // TESTSIGNALCHECKER_H
