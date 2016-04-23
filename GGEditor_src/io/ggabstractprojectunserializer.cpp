#include "ggabstractprojectunserializer.h"
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>

void GGAbstractProjectUnserializer::setSceneId(GGScene *sc, GG::SceneID id)
{
    sc->m_id = id;
}

void GGAbstractProjectUnserializer::setPageId(GGPage *p, GG::PageID id)
{
    p->m_id = id;
}

void GGAbstractProjectUnserializer::setConnectionId(GGConnection *c, GG::ConnectionID id)
{
    c->m_id = id;
}

void GGAbstractProjectUnserializer::resolveConnectionPages(GGConnection *conn, GGPage *source, GGPage *dest)
{
    conn->m_src = source;
    conn->m_dest = dest;

}
