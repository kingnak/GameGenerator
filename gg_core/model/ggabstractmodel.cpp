#include "ggabstractmodel.h"
#include "ggpage.h"
#include "ggconnection.h"

void GGAbstractModel::notifyPageUpdate(GG::PageID id, PageSections sections)
{
    if (GGPage *p = getPage(id)) {
        emit pageUpdated(p, sections);
    }
}

GGSearchResultList GGAbstractModel::search(GGSearchRequest req) const
{
    GGSearchResultList res;
    foreach (const GGPage *p, getPages()) {
        if (p->match(req, res)) {
            // -1 mean find all
            if (req.maxResults() >= 0 && res.size() >= req.maxResults()) {
                break;
            }
        }
    }
    return res;
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
