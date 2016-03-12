#include "ggeditcommandfactory.h"
#include "ggmodelpagecommands.h"
#include "ggmodelconnectioncommands.h"
#include "ggmodelgeneralcommands.h"
#include <model/ggpage.h>
#include <model/ggmappedlink.h>

GGEditCommandFactory::GGEditCommandFactory(GGEditModel *model)
    : m_model(model)
{

}

GGCreatePageCmd *GGEditCommandFactory::createStartPage()
{
    return new GGCreatePageCmd(m_model, GGCreatePageCmd::StartPage);
}

GGCreatePageCmd *GGEditCommandFactory::createEndPage()
{
    return new GGCreatePageCmd(m_model, GGCreatePageCmd::EndPage);
}

GGCreatePageCmd *GGEditCommandFactory::createConditionPage()
{
    return new GGCreatePageCmd(m_model, GGCreatePageCmd::ConditionPage);
}

GGCreatePageCmd *GGEditCommandFactory::createActionPage()
{
    return new GGCreatePageCmd(m_model, GGCreatePageCmd::ActionPage);
}

GGCreatePageCmd *GGEditCommandFactory::createDecisionPage()
{
    return new GGCreatePageCmd(m_model, GGCreatePageCmd::DecisionPage);
}

GGDeletePageCmd *GGEditCommandFactory::deletePage(GGPage *page)
{
    return new GGDeletePageCmd(m_model, page);
}

GGCreateConnectionCmd *GGEditCommandFactory::createConnection(GGPage *src, GGPage *dest, GGConnectionSlot slot)
{
    return new GGCreateConnectionCmd(m_model, slot, src, dest);
}

GGExchangeConnectionCmd *GGEditCommandFactory::exchangeConnection(GGPage *src, GGPage *dest, GGConnectionSlot slot)
{
    return new GGExchangeConnectionCmd(m_model, src, dest, slot);
}

GGDeleteConnectionCmd *GGEditCommandFactory::deleteConnection(GGConnection *conn)
{
    return new GGDeleteConnectionCmd(m_model, conn);
}

GGSetPageStringCmd *GGEditCommandFactory::setPageName(GGPage *p, QString name)
{
    return new GGSetPageStringCmd(m_model, p, name, GGSetPageStringCmd::Name);
}

GGSetPageStringCmd *GGEditCommandFactory::setPageScene(GGPage *p, QString sceneName)
{
    return new GGSetPageStringCmd(m_model, p, sceneName, GGSetPageStringCmd::SceneName);
}

GGSetPageStringCmd *GGEditCommandFactory::setPageCaption(GGContentPage *p, QString caption)
{
    return new GGSetPageStringCmd(m_model, p, caption, GGSetPageStringCmd::Caption);
}

GGExchangeContentCmd *GGEditCommandFactory::exchangeContent(GGContentPage *p, GGContentElement *elem)
{
    return new GGExchangeContentCmd(m_model, p, elem);
}

GGSetActionLinkCmd *GGEditCommandFactory::setActionLink(GGActionPage *p, const GGLink &lnk)
{
    return new GGSetActionLinkCmd(m_model, p, lnk);
}

GGMappedLinkCmd *GGEditCommandFactory::addMappedLink(GGMappedContentPage *p)
{
    return new GGMappedLinkCmd(m_model, p, GGMappedLink::rectangle(QRect()), GGMappedLinkCmd::Add, -1);
}

GGMappedLinkCmd *GGEditCommandFactory::addMappedLink(GGMappedContentPage *p, const GGMappedLink &lnk)
{
    return new GGMappedLinkCmd(m_model, p, lnk, GGMappedLinkCmd::Add, -1);
}

GGMappedLinkCmd *GGEditCommandFactory::removeMappedLink(GGMappedContentPage *p, int idx)
{
    return new GGMappedLinkCmd(m_model, p, GGMappedLink::rectangle(QRect()), GGMappedLinkCmd::Remove, idx);
}

GGMappedLinkCmd *GGEditCommandFactory::setMappedLink(GGMappedContentPage *p, int idx, const GGMappedLink &lnk)
{
    return new GGMappedLinkCmd(m_model, p, lnk, GGMappedLinkCmd::Set, idx);
}

GGDecisionLinkCmd *GGEditCommandFactory::addDecisionLink(GGDecisionPage *p)
{
    return new GGDecisionLinkCmd(m_model, p, GGLink(), GGDecisionLinkCmd::Add, -1);
}

GGDecisionLinkCmd *GGEditCommandFactory::removeDecisionLink(GGDecisionPage *p, int idx)
{
    return new GGDecisionLinkCmd(m_model, p, GGLink(), GGDecisionLinkCmd::Remove, idx);
}

GGDecisionLinkCmd *GGEditCommandFactory::setDecisionLink(GGDecisionPage *p, int idx, const GGLink &lnk)
{
    return new GGDecisionLinkCmd(m_model, p, lnk, GGDecisionLinkCmd::Set, idx);
}

GGAddVariableCmd *GGEditCommandFactory::addVariable(const QString &name)
{
    return new GGAddVariableCmd(m_model, name);
}

GGRemoveVariableCmd *GGEditCommandFactory::removeVariable(const QString &name)
{
    return new GGRemoveVariableCmd(m_model, name);
}

GGChangeVariableCmd *GGEditCommandFactory::updateVariable(const QString &name, const GGVariable &newData)
{
    return new GGChangeVariableCmd(m_model, name, newData);
}

