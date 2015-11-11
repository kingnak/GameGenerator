#ifndef MODELSIGNALCHECKER_H
#define MODELSIGNALCHECKER_H

#include <QObject>

class GGAbstractModel;

class ModelSignalChecker : public QObject
{
    Q_OBJECT
public:
    explicit ModelSignalChecker(GGAbstractModel *m, QObject *parent = 0);

    bool verify(QString s, int pr, int pu, int cr, int cu, int pc, bool r = true);
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

#endif // MODELSIGNALCHECKER_H
