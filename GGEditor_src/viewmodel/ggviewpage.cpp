#include "ggviewpage.h"
#include "ggviewmodel.h"

GGViewPage::GGViewPage(GGPage *page, GGViewModel *model, QRect bounds)
    : m_page(page),
      m_model(model),
      m_bounds(bounds)
{

}

void GGViewPage::setBounds(QRect r)
{
    m_bounds = r;
    m_model->notifyViewPageUpdate(this);
}
