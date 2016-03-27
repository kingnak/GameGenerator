#include "ggviewcommandfactory.h"
#include <viewmodel/ggviewmodel.h>
#include "ggviewcommands.h"

GGViewCommandFactory::GGViewCommandFactory(GGViewModel *model)
    : GGEditCommandFactory(model->editModel()), m_model(model)
{

}

GGCreateViewPageCmd *GGViewCommandFactory::createStartPage(GGScene *scene, const QRect &bounds)
{
    return new GGCreateViewPageCmd(m_model, scene, GGCreatePageCmd::StartPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createEndPage(GGScene *scene, const QRect &bounds)
{
    return new GGCreateViewPageCmd(m_model, scene, GGCreatePageCmd::EndPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createConditionPage(GGScene *scene, const QRect &bounds)
{
    return new GGCreateViewPageCmd(m_model, scene, GGCreatePageCmd::ConditionPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createActionPage(GGScene *scene, const QRect &bounds)
{
    return new GGCreateViewPageCmd(m_model, scene, GGCreatePageCmd::ActionPage, bounds);
}

GGCreateViewPageCmd *GGViewCommandFactory::createDecisionPage(GGScene *scene, const QRect &bounds)
{
    return new GGCreateViewPageCmd(m_model, scene, GGCreatePageCmd::DecisionPage, bounds);
}

GGDeleteViewPageCmd *GGViewCommandFactory::deletePage(GGViewPage *page)
{
    return new GGDeleteViewPageCmd(m_model, page);
}

GGMoveViewPageCmd *GGViewCommandFactory::movePage(GGViewPage *page, const QRect &pos)
{
    return new GGMoveViewPageCmd(m_model, page, pos);
}

GGCreateViewConnectionCmd *GGViewCommandFactory::createConnection(GGViewPage *src, GGViewPage *dest, const GGConnectionSlot &slot)
{
    return new GGCreateViewConnectionCmd(m_model, slot, src, dest);
}

GGDeleteViewConnectionCmd *GGViewCommandFactory::deleteConnection(GGViewConnection *conn)
{
    return new GGDeleteViewConnectionCmd(m_model, conn);
}
