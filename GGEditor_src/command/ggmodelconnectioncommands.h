#ifndef GGCMODELCONNECTIONCOMMMANDS_H
#define GGCMODELCONNECTIONCOMMMANDS_H

#include "ggabstractmodelcommand.h"
#include <model/ggconnectionslot.h>

class GGConnection;
class GGPage;
class GGCommandGroup;

class GGCreateConnectionCmd : public GGAbstractModelCommand
{
public:
    GGCreateConnectionCmd(GGEditModel *model, GGConnectionSlot slot, GGPage *src, GGPage *dest);
    ~GGCreateConnectionCmd();

    QString description() const;

    GGConnection *createdConnection();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGConnection *m_createdConn;
    GGConnectionSlot m_slot;
    GGPage *m_src;
    GGPage *m_dest;
};

///////////////////////////

class GGDeleteConnectionCmd : public GGAbstractModelCommand
{
public:
    GGDeleteConnectionCmd(GGEditModel *model, GGConnection *conn);
    ~GGDeleteConnectionCmd();

    QString description() const;

    GGConnection *deletedConnection();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGConnection *m_deletedConn;
    GGConnectionSlot m_slot;
};

///////////////////////////

class GGExchangeConnectionCmd : public GGAbstractModelCommand
{
public:
    GGExchangeConnectionCmd(GGEditModel *model, GGPage *src, GGPage *dest, GGConnectionSlot slot);
    ~GGExchangeConnectionCmd();

    QString description() const;

    GGConnection *newConnection();
    GGConnection *oldConnection();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGPage *m_src;
    GGPage *m_dest;
    GGDeleteConnectionCmd *m_del;
    GGCreateConnectionCmd *m_cre;
    GGCommandGroup *m_grp;
    GGConnectionSlot m_slot;
};

#endif // GGCMODELCONNECTIONCOMMMANDS_H
