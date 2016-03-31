#include "ggscene.h"
#include "ggpage.h"

GGScene::GGScene()
    : m_id(GG::InvalidSceneId),
      m_model(NULL)
{

}

GGScene::~GGScene()
{

}

QSet<GGPage *> GGScene::pages() const
{
    return m_pages;
}

void GGScene::addPage(GGPage *page)
{
    Q_ASSERT(page);
    Q_ASSERT(page->scene() == this);
    Q_ASSERT(!m_pages.contains(page));
    Q_ASSERT(page->model() == this->model());
    m_pages << page;
}

void GGScene::removePage(GGPage *page)
{
    Q_ASSERT(page);
    Q_ASSERT(page->scene() == this);
    Q_ASSERT(m_pages.contains(page));
    m_pages.remove(page);
}
