#ifndef GGVIEWCOMMANDFACTORY_H
#define GGVIEWCOMMANDFACTORY_H

#include <QRect>
#include <command/ggeditcommandfactory.h>
#include "model/ggmappedlink.h"
#include "model/ggconnectionslot.h"

class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGCreateViewPageCmd;
class GGDeleteViewPageCmd;
class GGCreateViewConnectionCmd;
//class GGExchangeViewConnectionCmd;
class GGDeleteViewConnectionCmd;
/*
class GGSetViewPageStringCmd;
class GGExchangeViewContentCmd;
class GGSetViewActionLinkCmd;
class GGViewMappedLinkCmd;
class GGViewDecisionLinkCmd;
*/

class GGViewCommandFactory : public GGEditCommandFactory
{
public:
    GGViewCommandFactory(GGViewModel *model);

    GGCreateViewPageCmd *createStartPage(QRect bounds);
    GGCreateViewPageCmd *createEndPage(QRect bounds);
    GGCreateViewPageCmd *createConditionPage(QRect bounds);
    GGCreateViewPageCmd *createActionPage(QRect bounds);
    GGCreateViewPageCmd *createDecisionPage(QRect bounds);

    GGDeleteViewPageCmd *deletePage(GGViewPage *page);

    GGCreateViewConnectionCmd *createConnection(GGViewPage *src, GGViewPage *dest, GGConnectionSlot slot);
    //GGExchangeViewConnectionCmd *exchangeConnection(GGViewPage *src, GGViewPage *dest, GGConnectionSlot slot);
    GGDeleteViewConnectionCmd *deleteConnection(GGViewConnection *conn);

    /*
    GGSetViewPageStringCmd *setPageName(GGViewPage *p, QString name);
    GGSetViewPageStringCmd *setPageScene(GGViewPage *p, QString sceneName);
    GGSetViewPageStringCmd *setPageCaption(GGViewPage *p, QString caption);

    GGExchangeViewContentCmd *exchangeContent(GGViewPage *p, GGContentElement *elem);

    GGSetViewActionLinkCmd *setActionLink(GGViewPage *p, const GGLink &lnk);

    GGViewMappedLinkCmd *addMappedLink(GGViewPage *p);
    GGViewMappedLinkCmd *removeMappedLink(GGViewPage *p, int idx);
    GGViewMappedLinkCmd *setMappedLink(GGViewPage *p, int idx, const GGMappedLink &lnk);

    GGViewDecisionLinkCmd *addDecisionLink(GGViewPage *p);
    GGViewDecisionLinkCmd *removeDecisionLink(GGViewPage *p, int idx);
    GGViewDecisionLinkCmd *setDecisionLink(GGViewPage *p, int idx, const GGLink &lnk);
    */

private:
    GGViewModel *m_model;

};

#endif // GGVIEWCOMMANDFACTORY_H
