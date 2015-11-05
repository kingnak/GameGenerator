#ifndef GGABSTRACTCOMMANDFACTORY_H
#define GGABSTRACTCOMMANDFACTORY_H

#include <QtGlobal>

class GGPage;

class GGCreatePageCmd;
class GGDeletePageCmd;

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

};

#endif // GGABSTRACTCOMMANDFACTORY_H
