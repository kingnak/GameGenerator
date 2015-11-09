#ifndef GGCMODELCONNECTIONCOMMMANDS_H
#define GGCMODELCONNECTIONCOMMMANDS_H

#include "ggabstractmodelcommand.h"
#include <model/ggconnectionslot.h>

class GGConnection;
class GGPage;

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

#endif // GGCMODELCONNECTIONCOMMMANDS_H
