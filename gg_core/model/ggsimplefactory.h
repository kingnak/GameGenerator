#ifndef GGSIMPLEFACTORY_H
#define GGSIMPLEFACTORY_H

#include "ggabstractfactory.h"

class GG_CORESHARED_EXPORT GGSimpleFactory : public GGAbstractFactory
{
public:
    GGSimpleFactory();

    virtual GGScene *createScene();

    virtual GGConditionPage *createConditionPage(GG::SceneID scene);
    virtual GGStartPage *createStartPage(GG::SceneID scene) ;
    virtual GGEndPage *createEndPage(GG::SceneID scene);
    virtual GGActionPage *createActionPage(GG::SceneID scene);
    virtual GGDecisionPage *createDecisionPage(GG::SceneID scene);

    virtual GGConnection *createConnection(GG::PageID src, GG::PageID dest);
};

#endif // GGSIMPLEFACTORY_H
