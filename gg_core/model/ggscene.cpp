#include "ggscene.h"
#include "ggpage.h"
#include <ggutilities.h>

GGScene::GGScene()
    : m_id(GG::InvalidSceneId),
      m_model(NULL)
{

}

GGScene::~GGScene()
{

}

void GGScene::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        if (m_model) {
            m_model->notifySceneUpdate(m_id);
        }
    }
}

void GGScene::setMediaDir(const QString &dir)
{
    QString dirName = GGUtilities::sanatizeFileName(dir);
    if (dirName != m_mediaDir) {
        m_mediaDir = dirName;
        if (m_model) {
            m_model->notifySceneUpdate(m_id);
        }
    }
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
