#include "ggabstractmodel.h"
#include "ggscene.h"
#include "ggpage.h"
#include "ggconnection.h"

void GGAbstractModel::notifyPageUpdate(GG::PageID id, PageSections sections)
{
    if (GGPage *p = getPage(id)) {
        emit pageUpdated(p, sections);
    }
}

void GGAbstractModel::notifySceneUpdate(GG::SceneID id)
{
    if (GGScene *s = getScene(id)) {
        emit sceneUpdated(s);
    }
}

GGSearchResult GGAbstractModel::search(GGSearchRequest req) const
{
    GGSearchResult res(req, this);
    QList<GGPage*> pages;
    if (req.scene() == GG::InvalidSceneId) {
        pages = getPages();
    } else if (getScene(req.scene())) {
        pages = getScene(req.scene())->pages().toList();
    }

    foreach (const GGPage *p, pages) {
        if (p->match(req, res)) {
            // -1 mean find all
            if (req.maxResults() >= 0 && res.count() >= req.maxResults()) {
                break;
            }
        }
    }
    return res;
}

void GGAbstractModel::setSceneId(GGScene *scene, GG::SceneID id)
{
    Q_ASSERT(scene->m_model == NULL || scene->m_model == this);
    scene->m_id = id;
    scene->m_model = this;
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

void GGAbstractModel::unsetModel(GGScene *scene) const
{
    scene->m_model = NULL;
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

void GGAbstractModel::resolvePageScene(GGPage *page, GGScene *scene) const
{
    page->m_scene = scene;
}
