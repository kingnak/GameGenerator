#ifndef GGVIEWMODEL_H
#define GGVIEWMODEL_H

#include <gg_definitions.h>
#include <QObject>
#include <QMap>

class GGEditModel;
class GGScene;
class GGPage;
class GGConnection;
class GGViewScene;
class GGViewPage;
class GGViewConnection;

class GGViewModel : public QObject
{
    Q_OBJECT
public:
    explicit GGViewModel(GGEditModel *model, QObject *parent = 0);
    ~GGViewModel();

    GGEditModel *editModel() { return m_model; }

    GGViewPage *getViewPageForPage(GGPage *page, GG::SceneID scene);
    GGViewConnection *getViewConectionForConnection(GGConnection *conn, GG::SceneID scene);

//    bool registerPage(GGViewPage *page);

    void purgeRecycledPage(GGPage *page);
    void purgeRecycledConnection(GGConnection *conn);

    void notifyViewPageUpdate(GGViewPage *page);

signals:
    void viewSceneRegistered(GGViewScene *vs);
    void viewSceneUnregistered(GGViewScene *vs);
    void viewPageRegistered(GGViewPage *vp);
    void viewPageUnregistered(GGViewPage *vp);
    void viewConnectionRegistered(GGViewConnection *vc);
    void viewConnectionUnregistered(GGViewConnection *vc);
    void pageUpdated(GGViewPage *vp);
    void viewPageUpdated(GGViewPage *vp);

private slots:
    void regScene(GGScene *scene);
    void unregScene(GG::SceneID id, GGScene *scene);
    void regPage(GGPage *page);
    void unregPage(GG::PageID id, GGPage *page);
    void regConn(GGConnection *conn);
    void unregConn(GG::ConnectionID id, GGConnection *conn);
    void updPage(GGPage *page);

private:
    void unregViewPage(GG::PageID pageId, GG::SceneID sceneId);
    void doRegConn(GGConnection *conn, GG::SceneID sceneId);
    void doUnregConn(GGConnection *conn, GG::SceneID sceneId);
    bool hasConnectionsInScene(GGPage *page, GG::SceneID scene);

private:
    GGEditModel *m_model;

    QMap<GG::SceneID, GGViewScene *> m_sceneMap;

    typedef QMap<GG::PageID, GGViewPage *> PageMap;
    typedef QMap<GG::ConnectionID, GGViewConnection *> ConnectionMap;
    QMap<GG::SceneID, PageMap> m_pageMap;
    QMap<GG::SceneID, ConnectionMap> m_connectionMap;

    QMap<GG::SceneID, PageMap> m_pageRec;
    QMap<GG::SceneID, ConnectionMap> m_connectionRec;
};

#endif // GGVIEWMODEL_H
