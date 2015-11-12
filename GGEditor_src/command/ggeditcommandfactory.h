#ifndef GGEDITCOMMANDFACTORY_H
#define GGEDITCOMMANDFACTORY_H

#include "ggabstractmodelcommandfactory.h"

class GGEditModel;

class GGEditCommandFactory : public GGAbstractModelCommandFactory
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
    GGExchangeConnectionCmd *exchangeConnection(GGPage *src, GGPage *dest, GGConnectionSlot slot);
    GGDeleteConnectionCmd *deleteConnection(GGConnection *conn);

private:
    GGEditModel *m_model;
};

#endif // GGEDITCOMMANDFACTORY_H
