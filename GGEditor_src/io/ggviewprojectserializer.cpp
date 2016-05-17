#include "ggviewprojectserializer.h"
#include <io/ggserialization.hpp>
#include <io/ggserializationprocessor.h>
#include <io/ggabstractserializationwriter.h>
#include <model/ggeditproject.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewscene.h>
#include <viewmodel/ggviewpage.h>
#include <viewmodel/ggviewconnection.h>

GGViewProjectSerializer::GGViewProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor)
    : GGBasicProjectSerializer(writer, processor),
      m_viewModel(NULL)
{

}

bool GGViewProjectSerializer::saveProject(GGEditProject *project, GGViewModel *viewModel)
{
    Q_ASSERT(project->editModel() == viewModel->editModel());
    if (project->editModel() != viewModel->editModel()) {
        return false;
    }
    m_viewModel = viewModel;
    return GGBasicProjectSerializer::saveProject(project);
}

bool GGViewProjectSerializer::injectSceneData(GGScene *scene, QVariantMap &v)
{
    GGViewScene *sc = m_viewModel->getViewSceneForScene(scene);
    v["scenePosition"] << sc->loadPosition();
    return true;
}

bool GGViewProjectSerializer::injectPageData(GGPage *page, QVariantMap &v)
{
    bool ok = true;
    QVariant l;
    QList<GGViewPage *> lst = m_viewModel->getAllViewPagesForPage(page);
    foreach (GGViewPage *vp, lst) {
        if (vp->viewSceneId() == page->sceneId()) {
            l << serializeViewPage(vp, ok);
            // Here we could remove the scene id for own view page...
        } else {
            m_foreignPages[page->id()] << vp;
        }
    }
    v["view"] << l;
    return ok;
}

bool GGViewProjectSerializer::injectConnectionData(GGConnection *connection, QVariantMap &v)
{
    bool ok = true;
    QVariant l;
    QList<GGViewConnection *> lst = m_viewModel->getAllViewConnectionsForConnection(connection);
    foreach (GGViewConnection *vc, lst) {
        if (vc->viewSceneId() == connection->source()->sceneId()){
            l << serializeViewConnection(vc, ok);
            // Here we could remove the scene id for own view connection...
        } else {
            m_foreignConns[connection->id()] << vc;
        }
    }
    v["view"] << l;
    return ok;
}

bool GGViewProjectSerializer::finalizeScene(GGScene *scene)
{
    Q_UNUSED(scene);

    bool ok = true;
    // Foreign pages
    {
        ok &= m_writer->writeForeignPagesStart();

        foreach (GG::PageID id, m_foreignPages.keys()) {
            QVariantMap map;
            map["pageId"] << id;
            ok &= m_processor->processPageRef(map["pageId"]);

            QVariantList lst;
            foreach (GGViewPage *vp, m_foreignPages[id]) {
                lst << serializeViewPage(vp, ok);
            }

            map["viewPage"] << lst;

            QVariant v;
            v << map;
            ok &= m_writer->writeForeignPage(v);
        }
        ok &= m_writer->writeForeignPagesEnd();
    }
    m_foreignPages.clear();

    // Foreign connections
    {
        ok &= m_writer->writeForeignConnectionsStart();

        foreach (GG::ConnectionID id, m_foreignConns.keys()) {
            QVariantMap map;
            map["connectionId"] << id;
            ok &= m_processor->processConnectionRef(map["connectionId"]);

            QVariantList lst;
            foreach (GGViewConnection *vc, m_foreignConns[id]) {
                lst << serializeViewConnection(vc, ok);
            }

            map["viewConnection"] << lst;

            QVariant v;
            v << map;
            ok &= m_writer->writeForeignConnection(v);
        }
        ok &= m_writer->writeForeignConnectionsEnd();
    }
    m_foreignConns.clear();

    return ok;
}

QVariantMap GGViewProjectSerializer::serializeViewPage(GGViewPage *vp, bool &ok)
{
    QVariantMap map;
    map["scene"] << vp->viewSceneId();
    ok &= m_processor->processSceneRef(map["scene"]);
    map["rect"] << vp->bounds();
    return map;
}

QVariantMap GGViewProjectSerializer::serializeViewConnection(GGViewConnection *vc, bool &ok)
{
    QVariantMap map;
    map["scene"] << vc->viewSceneId();
    ok &= m_processor->processSceneRef(map["scene"]);
    return map;
}
