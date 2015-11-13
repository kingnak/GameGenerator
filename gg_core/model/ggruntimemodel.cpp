#include "ggruntimemodel.h"
#include "ggpage.h"
#include "ggconnection.h"
#include "ggabstractfactory.h"

GGRuntimeModel::GGRuntimeModel(GGAbstractFactory *factory, QObject *parent)
    : GGAbstractModel(parent),
      m_factory(factory)
{

}

GGRuntimeModel::~GGRuntimeModel()
{
    qDeleteAll(m_connections);
    qDeleteAll(m_pages);
    delete m_factory;
}

GGAbstractFactory *GGRuntimeModel::factory()
{
    return m_factory;
}

GGPage *GGRuntimeModel::getPage(GG::PageID id)
{
    return m_pages.value(id);
}

GGConnection *GGRuntimeModel::getConnection(GG::ConnectionID id)
{
    return m_connections.value(id);
}

QList<GGPage *> GGRuntimeModel::getPages()
{
    return m_pages.values();
}

QList<GGConnection *> GGRuntimeModel::getConnections()
{
    return m_connections.values();
}

bool GGRuntimeModel::registerPageWithId(GGPage *page)
{
    if (!page || page->model()) {
        return false;
    }
    // Id must not be in use
    if (page->id() == GG::InvalidPageId || m_pages.contains(page->id())) {
        return false;
    }

    // Re-Associate with model
    setPageId(page, page->id());
    m_pages[page->id()] = page;

    emit pageRegistered(page);
    return true;
}

bool GGRuntimeModel::registerConnectionWithId(GGConnection *conn)
{
    if (!conn || conn->model()) {
        return false;
    }
    // Id must be valid
    if (conn->id() == GG::InvalidConnectionId || m_connections.contains(conn->id())) {
        return false;
    }

    if (!conn->destination() || !conn->source()) {
        Q_ASSERT_X(false, "GGModel::registerConnectionWithId", "Source and Dest msut be set");
        return false;
    }

    Q_ASSERT(conn->destination()->id() == conn->destinationId());
    Q_ASSERT(conn->source()->id() == conn->sourceId());

    // Pages must exist
    if (!m_pages.contains(conn->destination()->id()) || !m_pages.contains(conn->source()->id())) {
        return false;
    }

    // Re-Associate with model
    setConnectionId(conn, conn->id());
    m_connections[conn->id()] = conn;

    emit connectionRegistered(conn);
    return true;
}
