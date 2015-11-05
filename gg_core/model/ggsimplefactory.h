#ifndef GGSIMPLEFACTORY_H
#define GGSIMPLEFACTORY_H

#include "ggabstractfactory.h"

class GG_CORESHARED_EXPORT GGSimpleFactory : public GGAbstractFactory
{
public:
    GGSimpleFactory();

    virtual GGStartPage *createStartPage();
    virtual GGEndPage *createEndPage();

    virtual GGConnection *createConnection(GG::PageID src, GG::PageID dest);
};

#endif // GGSIMPLEFACTORY_H
