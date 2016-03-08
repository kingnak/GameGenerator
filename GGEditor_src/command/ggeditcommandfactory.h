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

    GGSetPageStringCmd *setPageName(GGPage *p, QString name);
    GGSetPageStringCmd *setPageScene(GGPage *p, QString sceneName);
    GGSetPageStringCmd *setPageCaption(GGContentPage *p, QString caption);

    GGExchangeContentCmd *exchangeContent(GGContentPage *p, GGContentElement *elem);

    GGSetActionLinkCmd *setActionLink(GGActionPage *p, const GGLink &lnk);

    GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p);
    GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p, const GGMappedLink &lnk);
    GGMappedLinkCmd *removeMappedLink(GGMappedContentPage *p, int idx);
    GGMappedLinkCmd *setMappedLink(GGMappedContentPage *p, int idx, const GGMappedLink &lnk);

    GGDecisionLinkCmd *addDecisionLink(GGDecisionPage *p);
    GGDecisionLinkCmd *removeDecisionLink(GGDecisionPage *p, int idx);
    GGDecisionLinkCmd *setDecisionLink(GGDecisionPage *p, int idx, const GGLink &lnk);

private:
    GGEditModel *m_model;
};

#endif // GGEDITCOMMANDFACTORY_H
