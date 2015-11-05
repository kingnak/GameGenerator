#ifndef GGABSTRACTCOMMANDFACTORY_H
#define GGABSTRACTCOMMANDFACTORY_H

#include <QtGlobal>

class GGCreatePageCmd;

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

};

#endif // GGABSTRACTCOMMANDFACTORY_H
