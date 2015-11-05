#ifndef GGRUNTIMEMODEL_H
#define GGRUNTIMEMODEL_H

#include "ggabstractmodel.h"
#include <QMap>

class GG_CORESHARED_EXPORT GGRuntimeModel : public GGAbstractModel
{
public:
    GGRuntimeModel(GGAbstractFactory *factory);
    ~GGRuntimeModel();

    GGAbstractFactory *factory();

    GGPage *getPage(GG::PageID id);
    GGConnection *getConnection(GG::ConnectionID id);

    bool registerPageWithId(GGPage *page);
    bool registerConnectionWithId(GGConnection *conn);

protected:
    GGAbstractFactory *m_factory;
    QMap<GG::PageID, GGPage *> m_pages;
    QMap<GG::ConnectionID, GGConnection *> m_connections;
};

#endif // GGRUNTIMEMODEL_H
