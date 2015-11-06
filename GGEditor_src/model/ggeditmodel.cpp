#include "ggeditmodel.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>

GGEditModel::GGEditModel(GGAbstractFactory *factory)
    : GGRuntimeModel(factory), m_nextPageId(0), m_nextConnId(0)
{

}

bool GGEditModel::registerNewPage(GGPage *page)
{
    if (!page || page->model() || page->id() != GG::InvalidPageId) {
        return false;
    }

    // Associate with model
    setPageId(page, m_nextPageId++);
    m_pages[page->id()] = page;

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

    return true;
}

bool GGEditModel::registerConnectionWithId(GGConnection *conn)
{
    bool ret = GGRuntimeModel::registerConnectionWithId(conn);
    if (ret) {
        // Store incomming connection
        m_incommingConns[conn->destinationId()] << conn->id();
    }
    return ret;
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
    foreach (GGConnection *c, aConns) {
        unregisterConnection(c->id());
    }

    m_pages.remove(id);
    unsetModel(ret);
    if (affectedConnections) *affectedConnections = aConns;
    return ret;
}

GGConnection *GGEditModel::unregisterConnection(GG::ConnectionID id)
{
    if (!m_connections.contains(id)) {
        return NULL;
    }
    GGConnection *ret = m_connections[id];
    // Remove from source
    ret->source()->removeConnection(ret);
    // Remove from incomming connections
    m_incommingConns[ret->destinationId()].remove(ret->id());
    m_connections.remove(id);
    unsetModel(ret);
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
