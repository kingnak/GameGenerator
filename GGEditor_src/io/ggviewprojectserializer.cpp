#include "ggviewprojectserializer.h"
#include <io/ggserialization.hpp>
#include <io/ggserializationprocessor.h>
#include <model/ggeditproject.h>
#include <viewmodel/ggviewmodel.h>
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

bool GGViewProjectSerializer::injectPageData(GGPage *page, QVariantMap &v)
{
    bool ok = true;
    QVariantList l;
    QList<GGViewPage *> lst = m_viewModel->getAllViewPagesForPage(page);
    foreach (GGViewPage *vp, lst) {
        QVariantMap map;
        map["scene"] << vp->viewSceneId();
        ok &= m_processor->processSceneRef(map["scene"]);
        map["rect"] << vp->bounds();
        l << map;
    }
    v["view"] << l;
    return ok;
}

bool GGViewProjectSerializer::injectConnectionData(GGConnection *connection, QVariantMap &v)
{
    bool ok = true;
    QVariantList l;
    QList<GGViewConnection *> lst = m_viewModel->getAllViewConnectionsForConnection(connection);
    foreach (GGViewConnection *vc, lst) {
        QVariantMap map;
        map["scene"] << vc->viewSceneId();
        ok &= m_processor->processSceneRef(map["scene"]);
        l << map;
    }
    v["view"] << l;
    return ok;
}
