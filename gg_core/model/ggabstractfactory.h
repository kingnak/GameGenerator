#ifndef GGABSTRACTFACTORY_H
#define GGABSTRACTFACTORY_H

#include <gg_core_global.h>
#include <gg_definitions.h>

class GGScene;
class GGConditionPage;
class GGStartPage;
class GGEndPage;
class GGActionPage;
class GGDecisionPage;
class GGConnection;
class GGLink;

class GG_CORESHARED_EXPORT GGAbstractFactory
{
    Q_DISABLE_COPY(GGAbstractFactory)

public:
    GGAbstractFactory() {}
    virtual ~GGAbstractFactory() {}

    virtual GGScene *createScene() = 0;

    virtual GGConditionPage *createConditionPage(GG::SceneID scene) = 0;
    virtual GGStartPage *createStartPage(GG::SceneID scene) = 0;
    virtual GGEndPage *createEndPage(GG::SceneID scene) = 0;
    virtual GGActionPage *createActionPage(GG::SceneID scene) = 0;
    virtual GGDecisionPage *createDecisionPage(GG::SceneID scene) = 0;

    virtual GGConnection *createConnection(GG::PageID src, GG::PageID dest) = 0;
};

#endif // GGABSTRACTFACTORY_H
