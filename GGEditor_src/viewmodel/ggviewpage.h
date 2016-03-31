#ifndef GGVIEWPAGE_H
#define GGVIEWPAGE_H

#include <QRect>
#include <gg_definitions.h>

class GGPage;
class GGViewModel;

class GGViewPage
{
public:
    GGViewPage(GGPage *page, GGViewModel *model, QRect bounds, GG::SceneID viewSceneId);

    GGPage *page() { return m_page; }
    QRect bounds() { return m_bounds; }
    void setBounds(QRect r);
    GG::SceneID viewSceneId() const { return m_sceneId; }

private:
    GGPage *m_page;
    GGViewModel *m_model;
    QRect m_bounds;
    GG::SceneID m_sceneId;
};

#endif // GGVIEWPAGE_H
