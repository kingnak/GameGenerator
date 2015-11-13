#include "ggviewcommandfactory.h"
#include <viewmodel/ggviewmodel.h>
#include "ggviewcommands.h"

GGViewCommandFactory::GGViewCommandFactory(GGViewModel *model)
    : m_model(model)
{

}

GGCreateViewPageCmd *GGViewCommandFactory::createStartPage(QRect bounds)
{
    return new GGCreateViewPageCmd(m_model, GGCreatePageCmd::StartPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createEndPage(QRect bounds)
{
    return new GGCreateViewPageCmd(m_model, GGCreatePageCmd::EndPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createConditionPage(QRect bounds)
{
    return new GGCreateViewPageCmd(m_model, GGCreatePageCmd::ConditionPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createActionPage(QRect bounds)
{
    return new GGCreateViewPageCmd(m_model, GGCreatePageCmd::ActionPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createDecisionPage(QRect bounds)
{
    return new GGCreateViewPageCmd(m_model, GGCreatePageCmd::DecisionPage, bounds);
}
