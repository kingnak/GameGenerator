#ifndef GGADDVARIABLECMD_H
#define GGADDVARIABLECMD_H

#include "ggabstractmodelcommand.h"
#include <model/ggvariable.h>

class GGAddVariableCmd : public GGAbstractModelCommand
{
public:
    GGAddVariableCmd(GGEditModel *model, const QString &name);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    QString m_name;
};

///////////////////

class GGRemoveVariableCmd : public GGAbstractModelCommand
{
public:
    GGRemoveVariableCmd(GGEditModel *model, const QString &name);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    QString m_name;
    GGVariable m_old;
};

////////////////////

class GGChangeVariableCmd : public GGAbstractModelCommand
{
public:
    GGChangeVariableCmd(GGEditModel *model, const QString &oldName, const GGVariable &var);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    bool doExchange(const GGVariable &o, const GGVariable &n);

private:
    GGVariable m_old;
    GGVariable m_new;
};

#endif // GGADDVARIABLECMD_H
