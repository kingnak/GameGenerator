#ifndef GGVIEWCOMMANDFACTORY_H
#define GGVIEWCOMMANDFACTORY_H

#include <command/ggeditcommandfactory.h>
#include "model/ggmappedlink.h"
#include "model/ggconnectionslot.h"

class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGCreateViewPageCmd;
class GGDeleteViewPageCmd;
class GGMoveViewPageCmd;
class GGCreateViewConnectionCmd;
class GGDeleteViewConnectionCmd;

class GGViewCommandFactory : public GGEditCommandFactory
{
public:
    GGViewCommandFactory(GGViewModel *model);

    GGCreateViewPageCmd *createStartPage(GGScene *scene, const QRect &bounds);
    GGCreateViewPageCmd *createEndPage(GGScene *scene, const QRect &bounds);
    GGCreateViewPageCmd *createConditionPage(GGScene *scene, const QRect &bounds);
    GGCreateViewPageCmd *createActionPage(GGScene *scene, const QRect &bounds);
    GGCreateViewPageCmd *createDecisionPage(GGScene *scene, const QRect &bounds);

    GGDeleteViewPageCmd *deletePage(GGViewPage *page);

    GGMoveViewPageCmd *movePage(GGViewPage *page, const QRect &pos);

    GGCreateViewConnectionCmd *createConnection(GGViewPage *src, GGViewPage *dest, const GGConnectionSlot &slot);
    GGDeleteViewConnectionCmd *deleteConnection(GGViewConnection *conn);
    using GGEditCommandFactory::deleteConnection;


private:
    GGViewModel *m_model;

};

#endif // GGVIEWCOMMANDFACTORY_H
