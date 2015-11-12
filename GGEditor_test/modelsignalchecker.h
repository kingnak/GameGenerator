#ifndef MODELSIGNALCHECKER_H
#define MODELSIGNALCHECKER_H

#include <QObject>

class GGAbstractModel;

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

#endif // MODELSIGNALCHECKER_H
