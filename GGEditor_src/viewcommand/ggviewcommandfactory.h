#ifndef GGVIEWCOMMANDFACTORY_H
#define GGVIEWCOMMANDFACTORY_H

#include <QRect>

class GGViewModel;
class GGCreateViewPageCmd;

class GGViewCommandFactory
{
public:
    GGViewCommandFactory(GGViewModel *model);

    GGCreateViewPageCmd *createStartPage(QRect bounds);
    GGCreateViewPageCmd *createEndPage(QRect bounds);
    GGCreateViewPageCmd *createConditionPage(QRect bounds);
    GGCreateViewPageCmd *createActionPage(QRect bounds);
    GGCreateViewPageCmd *createDecisionPage(QRect bounds);

private:
    GGViewModel *m_model;

};

#endif // GGVIEWCOMMANDFACTORY_H
