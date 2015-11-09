#ifndef GGABSTRACTFACTORY_H
#define GGABSTRACTFACTORY_H

#include <gg_core_global.h>
#include <gg_definitions.h>

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

    virtual GGConditionPage *createConditionPage() = 0;
    virtual GGStartPage *createStartPage() = 0;
    virtual GGEndPage *createEndPage() = 0;
    virtual GGActionPage *createActionPage() = 0;
    virtual GGDecisionPage *createDecisionPage() = 0;

    virtual GGConnection *createConnection(GG::PageID src, GG::PageID dest) = 0;
    virtual GGLink *createLink(GG::PageID src, GG::PageID dest) = 0;
};

#endif // GGABSTRACTFACTORY_H
