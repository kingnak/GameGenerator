#ifndef GGABSTRACTMODELCOMMANDFACTORY_H
#define GGABSTRACTMODELCOMMANDFACTORY_H

#include <QtGlobal>
#include <QString>
#include "ggabstractcommand.h"
#include <model/ggconnectionslot.h>

class GGPage;
class GGContentPage;
class GGActionPage;
class GGMappedContentPage;
class GGDecisionPage;

class GGContentElement;
class GGLink;
class GGMappedLink;
class GGVariable;

class GGCreatePageCmd;
class GGDeletePageCmd;
class GGCreateConnectionCmd;
class GGDeleteConnectionCmd;
class GGExchangeConnectionCmd;
class GGSetPageStringCmd;
class GGExchangeContentCmd;
class GGSetActionLinkCmd;
class GGMappedLinkCmd;
class GGDecisionLinkCmd;
class GGAddVariableCmd;
class GGRemoveVariableCmd;
class GGChangeVariableCmd;

class GGAbstractModelCommandFactory : public GGAbstractCommandFactory
{
public:
    GGAbstractModelCommandFactory() {}

    virtual GGCreatePageCmd *createStartPage() = 0;
    virtual GGCreatePageCmd *createEndPage() = 0;
    virtual GGCreatePageCmd *createConditionPage() = 0;
    virtual GGCreatePageCmd *createActionPage() = 0;
    virtual GGCreatePageCmd *createDecisionPage() = 0;

    virtual GGDeletePageCmd *deletePage(GGPage *page) = 0;

    virtual GGCreateConnectionCmd *createConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot) = 0;
    virtual GGExchangeConnectionCmd *exchangeConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot) = 0;
    virtual GGDeleteConnectionCmd *deleteConnection(GGConnection *conn) = 0;

    virtual GGSetPageStringCmd *setPageName(GGPage *p, const QString &name) = 0;
    virtual GGSetPageStringCmd *setPageScene(GGPage *p, const QString &sceneName) = 0;
    virtual GGSetPageStringCmd *setPageCaption(GGContentPage *p, const QString &caption) = 0;

    virtual GGExchangeContentCmd *exchangeContent(GGContentPage *p, GGContentElement *elem) = 0;

    virtual GGSetActionLinkCmd *setActionLink(GGActionPage *p, const GGLink &lnk) = 0;

    virtual GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p) = 0;
    virtual GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p, const GGMappedLink &lnk) = 0;
    virtual GGMappedLinkCmd *removeMappedLink(GGMappedContentPage *p, int idx) = 0;
    virtual GGMappedLinkCmd *setMappedLink(GGMappedContentPage *p, int idx, const GGMappedLink &lnk) = 0;

    virtual GGDecisionLinkCmd *addDecisionLink(GGDecisionPage *p) = 0;
    virtual GGDecisionLinkCmd *removeDecisionLink(GGDecisionPage *p, int idx) = 0;
    virtual GGDecisionLinkCmd *setDecisionLink(GGDecisionPage *p, int idx, const GGLink &lnk) = 0;

    virtual GGAddVariableCmd *addVariable(const QString &name) = 0;
    virtual GGRemoveVariableCmd *removeVariable(const QString &name) = 0;
    virtual GGChangeVariableCmd *updateVariable(const QString &name, const GGVariable &newData) = 0;
};

#endif // GGABSTRACTMODELCOMMANDFACTORY_H
