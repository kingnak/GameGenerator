#include "ggsimplefactory.h"
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGSimpleFactory::GGSimpleFactory()
{

}

GGScene *GGSimpleFactory::createScene()
{
    return new GGScene();
}

GGConditionPage *GGSimpleFactory::createConditionPage(GG::SceneID scene)
{
    return new GGConditionPage(scene);
}

GGStartPage *GGSimpleFactory::createStartPage(GG::SceneID scene)
{
    return new GGStartPage(scene);
}

GGEndPage *GGSimpleFactory::createEndPage(GG::SceneID scene)
{
    return new GGEndPage(scene);
}

GGActionPage *GGSimpleFactory::createActionPage(GG::SceneID scene)
{
    return new GGActionPage(scene);
}

GGDecisionPage *GGSimpleFactory::createDecisionPage(GG::SceneID scene)
{
    return new GGDecisionPage(scene);
}

GGConnection *GGSimpleFactory::createConnection(GG::PageID src, GG::PageID dest)
{
    return new GGConnection(src, dest);
}
