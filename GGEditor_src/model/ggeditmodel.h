#ifndef GGEDITMODEL_H
#define GGEDITMODEL_H

#include <gg_definitions.h>
#include <QMap>
#include <QSet>
#include <model/ggruntimemodel.h>

class GGScene;
class GGPage;
class GGConnection;

class GGEditModel : public GGRuntimeModel
{
public:
    explicit GGEditModel(GGAbstractFactory *factory, GGAbstractMediaResolver *resolver, QObject *parent = 0);

    bool registerNewScene(GGScene *scene);

    bool registerNewPage(GGPage *page);

    bool registerNewConnection(GGConnection *conn);
    bool registerConnectionWithId(GGConnection *conn);

    GGScene *unregisterScene(GG::SceneID id);
    GGPage *unregisterPage(GG::PageID id, QList<GGConnection *> *affectedConnections = NULL);
    GGConnection *unregisterConnection(GG::ConnectionID id);

    QList<GGConnection *> getPageIncommingConnections(GGPage *page) const;
    QList<GGConnection *> getPageAllConnections(GGPage *page) const;

private:
    QMap<GG::PageID, QSet<GG::ConnectionID> > m_incommingConns;

    GG::SceneID m_nextSceneId;
    GG::PageID m_nextPageId;
    GG::ConnectionID m_nextConnId;

    GGPage *m_unregisteringPage;
};

#endif // GGEDITMODEL_H
