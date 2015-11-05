#include "ggeditcommandfactory.h"
#include "ggmodelcommands.h"

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

