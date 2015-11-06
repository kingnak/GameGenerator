#ifndef GGABSTRACTMODEL_H
#define GGABSTRACTMODEL_H

#include <gg_core_global.h>
#include <gg_definitions.h>

class GGPage;
class GGConnection;
class GGAbstractFactory;

class GG_CORESHARED_EXPORT GGAbstractModel
{
    Q_DISABLE_COPY(GGAbstractModel)

public:
    GGAbstractModel() {}
    virtual ~GGAbstractModel() {}
    virtual GGAbstractFactory *factory() = 0;

    virtual GGPage *getPage(GG::PageID id) = 0;
    virtual GGConnection *getConnection(GG::ConnectionID id) = 0;

    virtual QList<GGPage *> getPages() = 0;
    virtual QList<GGConnection *> getConnections() = 0;

    virtual bool registerPageWithId(GGPage *page) = 0;
    virtual bool registerConnectionWithId(GGConnection *conn) = 0;

protected:
    void setPageId(GGPage *page, GG::PageID id);
    void setConnectionId(GGConnection *connection, GG::ConnectionID id);
    void unsetModel(GGPage *page) const;
    void unsetModel(GGConnection *connection) const;
    void resolveConnectionPages(GGConnection *connection, GGPage *source, GGPage *destination) const;
};

#endif // GGABSTRACTMODEL_H

