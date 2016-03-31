#ifndef GGVIEWSCENE_H
#define GGVIEWSCENE_H

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

private:
    GGScene *m_scene;
    GGViewModel *m_model;
};

#endif // GGVIEWSCENE_H
