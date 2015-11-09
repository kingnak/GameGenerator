#ifndef GGEDITCOMMANDFACTORY_H
#define GGEDITCOMMANDFACTORY_H

#include "ggabstractcommandfactory.h"

class GGEditModel;

class GGEditCommandFactory : public GGAbstractCommandFactory
{
public:
    GGEditCommandFactory(GGEditModel *model);

    GGCreatePageCmd *createStartPage();
    GGCreatePageCmd *createEndPage();
    GGCreatePageCmd *createConditionPage();
    GGCreatePageCmd *createActionPage();
    GGCreatePageCmd *createDecisionPage();

    GGDeletePageCmd *deletePage(GGPage *page);

    GGCreateConnectionCmd *createConnection(GGPage *src, GGPage *dest, GGConnectionSlot slot);

    GGDeleteConnectionCmd *deleteConnection(GGConnection *conn);

private:
    GGEditModel *m_model;
};

#endif // GGEDITCOMMANDFACTORY_H
