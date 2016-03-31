#include "ggviewpage.h"
#include "ggviewmodel.h"

GGViewPage::GGViewPage(GGPage *page, GGViewModel *model, QRect bounds, GG::SceneID viewSceneId)
    : m_page(page),
      m_model(model),
      m_bounds(bounds),
      m_sceneId(viewSceneId)
{

}

void GGViewPage::setBounds(QRect r)
{
    m_bounds = r;
    m_model->notifyViewPageUpdate(this);
}
