#include "ggviewconnection.h"

GGViewConnection::GGViewConnection(GGConnection *conn, GGViewModel *model, GG::SceneID viewSceneId)
    : m_conn(conn),
      m_model(model),
      m_sceneId(viewSceneId)
{

}

