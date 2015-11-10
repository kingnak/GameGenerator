#include "ggeditcommandfactory.h"
#include "ggmodelpagecommands.h"
#include "ggmodelconnectioncommands.h"

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

