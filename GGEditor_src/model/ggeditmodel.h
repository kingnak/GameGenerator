#ifndef GGEDITMODEL_H
#define GGEDITMODEL_H

#include <gg_definitions.h>
#include <QMap>
#include <QSet>
#include <model/ggruntimemodel.h>

class GGPage;
class GGConnection;

class GGEditModel : public GGRuntimeModel
{
public:
    explicit GGEditModel(GGAbstractFactory *factory, QObject *parent = 0);

    bool registerNewPage(GGPage *page);

    bool registerNewConnection(GGConnection *conn);
    bool registerConnectionWithId(GGConnection *conn);

    GGPage *unregisterPage(GG::PageID id, QList<GGConnection *> *affectedConnections = NULL);
    GGConnection *unregisterConnection(GG::ConnectionID id);

    QList<GGConnection *> getPageIncommingConnections(GGPage *page) const;
    QList<GGConnection *> getPageAllConnections(GGPage *page) const;

private:
    QMap<GG::PageID, QSet<GG::ConnectionID> > m_incommingConns;

    GG::PageID m_nextPageId;
    GG::ConnectionID m_nextConnId;

    GGPage *m_unregisteringPage;
};

#endif // GGEDITMODEL_H
