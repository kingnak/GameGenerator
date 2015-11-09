#ifndef GGSIMPLEFACTORY_H
#define GGSIMPLEFACTORY_H

#include "ggabstractfactory.h"

class GG_CORESHARED_EXPORT GGSimpleFactory : public GGAbstractFactory
{
public:
    GGSimpleFactory();

    virtual GGConditionPage *createConditionPage();
    virtual GGStartPage *createStartPage() ;
    virtual GGEndPage *createEndPage();
    virtual GGActionPage *createActionPage();
    virtual GGDecisionPage *createDecisionPage();

    virtual GGConnection *createConnection(GG::PageID src, GG::PageID dest);
};

#endif // GGSIMPLEFACTORY_H
