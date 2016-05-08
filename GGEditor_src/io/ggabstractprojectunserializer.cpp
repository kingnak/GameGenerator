#include "ggabstractprojectunserializer.h"
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggmediamanager.h>
#include <viewmodel/ggviewmodel.h>

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

bool GGAbstractProjectUnserializer::injectViewScene(GGViewModel *model, GGViewScene *vs)
{
    return model->injectViewScene(vs);
}

bool GGAbstractProjectUnserializer::injectViewPage(GGViewModel *model, GGViewPage *vp)
{
    return model->injectViewPage(vp);
}

bool GGAbstractProjectUnserializer::injectViewConnection(GGViewModel *model, GGViewConnection *vc)
{
    return model->injectViewConnection(vc);
}

bool GGAbstractProjectUnserializer::injectMedia(GGMediaManager *manager, const QString &id, const QString &path)
{
    return manager->injectMedia(id, path);
}
