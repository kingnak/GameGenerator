#include "ggabstractmodel.h"
#include "ggpage.h"
#include "ggconnection.h"

void GGAbstractModel::notifyPageUpdate(GG::PageID id)
{
    if (GGPage *p = getPage(id)) {
        emit pageUpdated(p);
    }
}

void GGAbstractModel::setPageId(GGPage *page, GG::PageID id)
{
    Q_ASSERT(page->m_model == NULL || page->m_model == this);
    page->m_id = id;
    page->m_model = this;
}

void GGAbstractModel::setConnectionId(GGConnection *connection, GG::ConnectionID id)
{
    Q_ASSERT(connection->m_model == NULL || connection->m_model == this);
    connection->m_id = id;
    connection->m_model = this;
}

void GGAbstractModel::unsetModel(GGPage *page) const
{
    page->m_model = NULL;
}

void GGAbstractModel::unsetModel(GGConnection *connection) const
{
    connection->m_model = NULL;
}

void GGAbstractModel::resolveConnectionPages(GGConnection *connection, GGPage *source, GGPage *destination) const
{
    connection->m_src = source;
    connection->m_dest = destination;
}
