#ifndef GGMODEL_H
#define GGMODEL_H

#include <gg_definitions.h>
#include <QMap>
#include <QSet>
#include <model/ggruntimemodel.h>

class GGPage;
class GGConnection;

class GGEditModel : public GGRuntimeModel
{
public:
    GGEditModel(GGAbstractFactory *factory);

    bool registerNewPage(GGPage *page);

    bool registerNewConnection(GGConnection *conn);
    bool registerConnectionWithId(GGConnection *conn);

    GGPage *unregisterPage(GG::PageID id, QList<GGConnection *> *affectedConnections = NULL);
    GGConnection *unregisterConnection(GG::ConnectionID id);

    QList<GGConnection *> getPageIncommingConnections(GGPage *page) const;

private:
    QMap<GG::PageID, QSet<GG::ConnectionID> > m_incommingConns;

    GG::PageID m_nextPageId;
    GG::ConnectionID m_nextConnId;
};

#endif // GGMODEL_H
