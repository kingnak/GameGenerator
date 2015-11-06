#ifndef GGABSTRACTCOMMANDFACTORY_H
#define GGABSTRACTCOMMANDFACTORY_H

#include <QtGlobal>
#include <QString>
#include "ggabstractcommand.h"
#include <model/ggconnectionslot.h>

class GGPage;

class GGCreatePageCmd;
class GGDeletePageCmd;
class GGCreateConnectionCmd;
class GGDeleteConnectionCmd;

class GGAbstractCommandFactory
{
    Q_DISABLE_COPY(GGAbstractCommandFactory)

public:
    GGAbstractCommandFactory() {}
    virtual ~GGAbstractCommandFactory() {}

    virtual GGCreatePageCmd *createStartPage() = 0;
    virtual GGCreatePageCmd *createEndPage() = 0;
    virtual GGCreatePageCmd *createConditionPage() = 0;
    virtual GGCreatePageCmd *createActionPage() = 0;
    virtual GGCreatePageCmd *createDecisionPage() = 0;

    virtual GGDeletePageCmd *deletePage(GGPage *page) = 0;

    virtual GGCreateConnectionCmd *createConnection(GGPage *src, GGPage *dest, GGConnectionSlot slot) = 0;
    virtual GGCreateConnectionCmd *createLink(GGPage *src, GGPage *dest, GGConnectionSlot slot) = 0;

    virtual GGDeleteConnectionCmd *deleteConnection(GGConnection *conn) = 0;

    static inline bool oneShotCommand(GGAbstractCommand *cmd, QString *error = NULL);

};

bool GGAbstractCommandFactory::oneShotCommand(GGAbstractCommand *cmd, QString *error)
{
    bool ret = cmd->execute();
    if (!ret && error) *error = cmd->error();
    delete cmd;
    return ret;
}

#endif // GGABSTRACTCOMMANDFACTORY_H
