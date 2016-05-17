#ifndef GGVIEWSCENE_H
#define GGVIEWSCENE_H

#include <QString>
#include <QPoint>

class GGScene;
class GGViewModel;

class GGViewScene
{
public:
    GGViewScene(GGScene *scene, GGViewModel *model)
        : m_scene(scene), m_model(model)
    {}

    GGScene *scene() const { return m_scene; }
    GGViewModel *model() const { return m_model; }

    QPoint loadPosition() const { return m_loadPosition; }
    void setLoadPosition(const QPoint &loadPosition) { m_loadPosition = loadPosition; }

private:
    GGScene *m_scene;
    GGViewModel *m_model;
    QPoint m_loadPosition;
};

#endif // GGVIEWSCENE_H
