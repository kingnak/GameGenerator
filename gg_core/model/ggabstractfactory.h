#ifndef GGABSTRACTFACTORY_H
#define GGABSTRACTFACTORY_H

#include <gg_core_global.h>
#include <gg_definitions.h>

class GGPage;
class GGStartPage;
class GGEndPage;
class GGConnection;

class GG_CORESHARED_EXPORT GGAbstractFactory
{
    Q_DISABLE_COPY(GGAbstractFactory)

public:
    GGAbstractFactory() {}
    virtual ~GGAbstractFactory() {}

    virtual GGStartPage *createStartPage() = 0;
    virtual GGEndPage *createEndPage() = 0;

    virtual GGConnection *createConnection(GG::PageID src, GG::PageID dest) = 0;
};

#endif // GGABSTRACTFACTORY_H
