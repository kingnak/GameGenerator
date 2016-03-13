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

    GGCreateConnectionCmd *createConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot);
    GGExchangeConnectionCmd *exchangeConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot);
    GGDeleteConnectionCmd *deleteConnection(GGConnection *conn);

    GGSetPageStringCmd *setPageName(GGPage *p, const QString &name);
    GGSetPageStringCmd *setPageScene(GGPage *p, const QString &sceneName);
    GGSetPageStringCmd *setPageCaption(GGContentPage *p, const QString &caption);

    GGExchangeContentCmd *exchangeContent(GGContentPage *p, GGContentElement *elem);

    GGSetActionLinkCmd *setActionLink(GGActionPage *p, const GGLink &lnk);

    GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p);
    GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p, const GGMappedLink &lnk);
    GGMappedLinkCmd *removeMappedLink(GGMappedContentPage *p, int idx);
    GGMappedLinkCmd *setMappedLink(GGMappedContentPage *p, int idx, const GGMappedLink &lnk);

    GGDecisionLinkCmd *addDecisionLink(GGDecisionPage *p);
    GGDecisionLinkCmd *removeDecisionLink(GGDecisionPage *p, int idx);
    GGDecisionLinkCmd *setDecisionLink(GGDecisionPage *p, int idx, const GGLink &lnk);

    GGAddVariableCmd *addVariable(const QString &name);
    GGRemoveVariableCmd *removeVariable(const QString &name);
    GGChangeVariableCmd *updateVariable(const QString &name, const GGVariable &newData);

private:
    GGEditModel *m_model;
};

#endif // GGEDITCOMMANDFACTORY_H
