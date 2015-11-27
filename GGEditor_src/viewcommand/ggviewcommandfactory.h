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
class GGMoveViewPageCmd;
class GGCreateViewConnectionCmd;
class GGDeleteViewConnectionCmd;

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

    GGMoveViewPageCmd *movePage(GGViewPage *page, QRect pos);

    GGCreateViewConnectionCmd *createConnection(GGViewPage *src, GGViewPage *dest, GGConnectionSlot slot);
    GGDeleteViewConnectionCmd *deleteConnection(GGViewConnection *conn);
    using GGEditCommandFactory::deleteConnection;


private:
    GGViewModel *m_model;

};

#endif // GGVIEWCOMMANDFACTORY_H
