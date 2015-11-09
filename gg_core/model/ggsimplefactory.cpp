#include "ggsimplefactory.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGSimpleFactory::GGSimpleFactory()
{

}

GGConditionPage *GGSimpleFactory::createConditionPage()
{
    return new GGConditionPage();
}

GGStartPage *GGSimpleFactory::createStartPage()
{
    return new GGStartPage();
}

GGEndPage *GGSimpleFactory::createEndPage()
{
    return new GGEndPage();
}

GGActionPage *GGSimpleFactory::createActionPage()
{
    return new GGActionPage();
}

GGDecisionPage *GGSimpleFactory::createDecisionPage()
{
    return new GGDecisionPage();
}

GGConnection *GGSimpleFactory::createConnection(GG::PageID src, GG::PageID dest)
{
    return new GGConnection(src, dest);
}
