#ifndef GGADDVARIABLECMD_H
#define GGADDVARIABLECMD_H

#include "ggabstractmodelcommand.h"
#include <model/ggvariable.h>
#include <style/ggstyle.h>

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

////////////////////

class GGSetBasicStyleCmd : public GGAbstractModelCommand
{
public:
    GGSetBasicStyleCmd(GGEditModel *model, const GGBasicStyle &style);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGBasicStyle m_new;
    GGBasicStyle m_old;
};

////////////////////

class GGAddStyleCmd : public GGAbstractModelCommand
{
public:
    GGAddStyleCmd(GGEditModel *model, const GGStyle &style);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGStyle m_style;
};

////////////////////

class GGRemoveStyleCmd : public GGAbstractModelCommand
{
public:
    GGRemoveStyleCmd(GGEditModel *model, const QString &style);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    QString m_name;
    GGStyle m_style;
};

#endif // GGADDVARIABLECMD_H
