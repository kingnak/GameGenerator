#ifndef GGVIEWCONNECTION_H
#define GGVIEWCONNECTION_H

#include <gg_definitions.h>

class GGConnection;
class GGViewModel;

class GGViewConnection
{
public:
    GGViewConnection(GGConnection *conn, GGViewModel *model, GG::SceneID viewSceneId);

    GG::SceneID viewSceneId() const { return m_sceneId; }

    GGConnection *connection() { return m_conn; }

private:
    GGConnection *m_conn;
    GGViewModel *m_model;
    GG::SceneID m_sceneId;
};

#endif // GGVIEWCONNECTION_H
