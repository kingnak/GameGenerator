#include "ggabstractprojectunserializer.h"
#include <model/ggscene.h>
#include <model/ggabstractmodel.h>

void GGAbstractProjectUnserializer::setSceneId(GGScene *sc, GG::SceneID id)
{
    sc->m_id = id;
}
