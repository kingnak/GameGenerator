#include "ggeditmodel.h"
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggabstractfactory.h>

GGEditModel::GGEditModel(GGAbstractFactory *factory, QObject *parent)
    : GGRuntimeModel(parent),
      m_factory(factory),
      m_nextSceneId(0),
      m_nextPageId(0),
      m_nextConnId(0),
      m_unregisteringPage(NULL)
{

}

GGEditModel::~GGEditModel()
{
    delete m_factory;
}

GGAbstractFactory *GGEditModel::factory()
{
    return m_factory;
}

bool GGEditModel::registerNewScene(GGScene *scene)
{
    Q_ASSERT(scene);
    Q_ASSERT(scene->id() == GG::InvalidSceneId);
    Q_ASSERT(!scene->model());
    if (!scene || scene->model() || scene->id() != GG::InvalidSceneId) {
        return false;
    }

    setSceneId(scene, m_nextSceneId++);
    m_scenes[scene->id()] = scene;

    emit sceneRegistered(scene);
    return true;
}

bool GGEditModel::registerNewPage(GGPage *page)
{
    Q_ASSERT(page);
    Q_ASSERT(page->id() == GG::InvalidPageId);
    Q_ASSERT(!page->model());
    if (!page || page->model() || page->id() != GG::InvalidPageId) {
        return false;
    }

    Q_ASSERT(!page->scene());
    if (page->scene()) {
        return false;
    }

    GGScene *scene = m_scenes.value(page->sceneId());
    Q_ASSERT(scene);
    if (!scene) {
        return false;
    }

    // Associate with model
    setPageId(page, m_nextPageId++);
    m_pages[page->id()] = page;

    // Resolve scene
    resolvePageScene(page, scene);
    scene->addPage(page);

    emit pageRegistered(page);
    return true;
}

bool GGEditModel::registerNewConnection(GGConnection *conn)
{
    if (!conn || conn->model() || conn->id() != GG::InvalidConnectionId) {
        return false;
    }

    if (conn->destination() || conn->source()) {
        Q_ASSERT_X(false, "GGModel::registerConnection", "Source / Destination must not be set!");
        return false;
    }

    // Pages must exist
    if (!m_pages.contains(conn->destinationId()) || !m_pages.contains(conn->sourceId())) {
        return false;
    }

    // Associate with model
    setConnectionId(conn, m_nextConnId++);
    m_connections[conn->id()] = conn;

    // Connect to pages
    resolveConnectionPages(conn, m_pages[conn->sourceId()], m_pages[conn->destinationId()]);

    // Store incomming connection
    m_incommingConns[conn->destinationId()] << conn->id();

    emit connectionRegistered(conn);
    return true;
}

bool GGEditModel::registerConnectionWithId(GGConnection *conn)
{
    // Don't emit the connectionRegistered signal by base class.
    // We emit it ourself, after inserting it in the incomming conn map.
    bool wasBlocked = this->blockSignals(true);
    bool ret = GGRuntimeModel::registerConnectionWithId(conn);
    if (!wasBlocked) this->blockSignals(false);

    if (ret) {
        // Store incomming connection
        m_incommingConns[conn->destinationId()] << conn->id();
        emit connectionRegistered(conn);
    }
    return ret;
}

GGScene *GGEditModel::unregisterScene(GG::SceneID id)
{
    GGScene *scene = m_scenes.value(id);
    if (scene) {
        // Can only unregister empty scenes
        if (scene->pages().isEmpty()) {
            m_scenes.remove(id);
            unsetModel(scene);
            emit sceneUnregistered(id, scene);
            return scene;
        }
    }
    return NULL;
}


GGPage *GGEditModel::unregisterPage(GG::PageID id, QList<GGConnection *> *affectedConnections)
{
    QList<GGConnection *> aConns;
    if (!m_pages.contains(id)) {
        return NULL;
    }

    GGPage *ret = m_pages[id];
    // Get all outgoing and incomming conns and remove them
    aConns = ret->getConnections();
    aConns += this->getPageIncommingConnections(ret);
    // Don't emit change signals for the page being removed
    m_unregisteringPage = ret;
    foreach (GGConnection *c, aConns) {
        unregisterConnection(c->id());
    }
    m_unregisteringPage = NULL;

    m_pages.remove(id);
    ret->scene()->removePage(ret);
    resolvePageScene(ret, NULL);
    unsetModel(ret);

    if (affectedConnections) *affectedConnections = aConns;

    emit pageUnregistered(id, ret);
    return ret;
}

GGConnection *GGEditModel::unregisterConnection(GG::ConnectionID id)
{
    if (!m_connections.contains(id)) {
        return NULL;
    }
    GGConnection *ret = m_connections[id];
    // Remove from source
    // If source is being unregistered, don't emit change signal
    if (ret->source() == m_unregisteringPage) {
        bool wasBlocked = this->blockSignals(true);
        ret->source()->removeConnection(ret);
        if (!wasBlocked) this->blockSignals(false);
    } else {
        ret->source()->removeConnection(ret);
    }
    // Remove from incomming connections
    m_incommingConns[ret->destinationId()].remove(ret->id());
    m_connections.remove(id);
    unsetModel(ret);

    emit connectionUnregistered(id, ret);
    return ret;
}

QList<GGConnection *> GGEditModel::getPageIncommingConnections(GGPage *page) const
{
    QList<GGConnection *> ret;
    if (!page || !m_pages.contains(page->id())) {
        return ret;
    }
    foreach (GG::ConnectionID cId, m_incommingConns.value(page->id())) {
        Q_ASSERT(m_connections.contains(cId));
        ret << m_connections[cId];
    }
    return ret;
}

QList<GGConnection *> GGEditModel::getPageAllConnections(GGPage *page) const
{
    return page->getConnections() + getPageIncommingConnections(page);
}
