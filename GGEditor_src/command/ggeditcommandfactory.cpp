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

GGCreateSceneCmd *GGEditCommandFactory::createScene(const QString &name, const QString &mediaDir)
{
    return new GGCreateSceneCmd(m_model, name, mediaDir);
}

GGDeleteSceneCmd *GGEditCommandFactory::deleteScene(GGScene *scene)
{
    return new GGDeleteSceneCmd(m_model, scene);
}

GGRenameSceneCmd *GGEditCommandFactory::renameScene(GGScene *scene, const QString &newName)
{
    return new GGRenameSceneCmd(m_model, scene, newName);
}

GGRenameSceneMediaDirCmd *GGEditCommandFactory::renameSceneMediaDir(GGScene *scene, const QString &newName)
{
    return new GGRenameSceneMediaDirCmd(m_model, scene, newName);
}

GGCreatePageCmd *GGEditCommandFactory::createStartPage(GGScene *scene)
{
    return new GGCreatePageCmd(m_model, scene, GGCreatePageCmd::StartPage);
}

GGCreatePageCmd *GGEditCommandFactory::createEndPage(GGScene *scene)
{
    return new GGCreatePageCmd(m_model, scene, GGCreatePageCmd::EndPage);
}

GGCreatePageCmd *GGEditCommandFactory::createConditionPage(GGScene *scene)
{
    return new GGCreatePageCmd(m_model, scene, GGCreatePageCmd::ConditionPage);
}

GGCreatePageCmd *GGEditCommandFactory::createActionPage(GGScene *scene)
{
    return new GGCreatePageCmd(m_model, scene, GGCreatePageCmd::ActionPage);
}

GGCreatePageCmd *GGEditCommandFactory::createDecisionPage(GGScene *scene)
{
    return new GGCreatePageCmd(m_model, scene, GGCreatePageCmd::DecisionPage);
}

GGDeletePageCmd *GGEditCommandFactory::deletePage(GGPage *page)
{
    return new GGDeletePageCmd(m_model, page);
}

GGCreateConnectionCmd *GGEditCommandFactory::createConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot)
{
    return new GGCreateConnectionCmd(m_model, slot, src, dest);
}

GGExchangeConnectionCmd *GGEditCommandFactory::exchangeConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot)
{
    return new GGExchangeConnectionCmd(m_model, src, dest, slot);
}

GGDeleteConnectionCmd *GGEditCommandFactory::deleteConnection(GGConnection *conn)
{
    return new GGDeleteConnectionCmd(m_model, conn);
}

GGSetPageStringCmd *GGEditCommandFactory::setPageName(GGPage *p, const QString &name)
{
    return new GGSetPageStringCmd(m_model, p, name, GGSetPageStringCmd::Name);
}

//GGSetPageStringCmd *GGEditCommandFactory::setPageScene(GGPage *p, const QString &sceneName)
//{
//    return new GGSetPageStringCmd(m_model, p, sceneName, GGSetPageStringCmd::SceneName);
//}

GGSetPageStringCmd *GGEditCommandFactory::setPageCaption(GGContentPage *p, const QString &caption)
{
    return new GGSetPageStringCmd(m_model, p, caption, GGSetPageStringCmd::Caption);
}

GGExchangeConditionCmd *GGEditCommandFactory::exchangeCondition(GGConditionPage *p, const GGCondition &cond)
{
    return new GGExchangeConditionCmd(m_model, p, cond);
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

