#include "ggsimplefactory.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGSimpleFactory::GGSimpleFactory()
{

}

GGStartPage *GGSimpleFactory::createStartPage()
{
    return new GGStartPage();
}

GGEndPage *GGSimpleFactory::createEndPage()
{
    return new GGEndPage();
}

GGConnection *GGSimpleFactory::createConnection(GG::PageID src, GG::PageID dest)
{
    return new GGConnection(src, dest);
}

