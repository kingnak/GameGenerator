#include "ggviewprojectunserializer.h"
#include <io/ggserialization.hpp>
#include <io/ggunserializationprocessor.h>
#include <model/ggeditmodel.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewscene.h>
#include <viewmodel/ggviewpage.h>
#include <viewmodel/ggviewconnection.h>

GGViewProjectUnserializer::GGViewProjectUnserializer(const QString &basePath, const QString &fileName, GGAbstractUnserializationReader *reader, GGUnserializationProcessor *processor)
    : GGBasicProjectUnserializer(basePath, fileName, reader, processor)
{
    m_viewModel = new GGViewModel(this->m_project->editModel());
}

GGViewProjectUnserializer::~GGViewProjectUnserializer()
{
    delete m_viewModel;
}

bool GGViewProjectUnserializer::load(QIODevice *device)
{
    m_vcMap.clear();
    m_vpMap.clear();
    return GGBasicProjectUnserializer::load(device);
}

GGViewModel *GGViewProjectUnserializer::loadedViewModel()
{
    return m_viewModel;
}

GGViewModel *GGViewProjectUnserializer::takeViewModel()
{
    GGViewModel *ret = m_viewModel;
    m_viewModel = NULL;
    return ret;
}

bool GGViewProjectUnserializer::unserializePage(QVariant page)
{
    bool ok = GGBasicProjectUnserializer::unserializePage(page);
    if (!ok) return false;
    QVariantMap map;
    page >> map;
    if (!map.contains("view")) return false;

    if (!m_processor->processPageRef(map["id"])) return false;
    GG::PageID pid = static_cast<GG::PageID> (map["id"].toUInt(&ok));
    if (!ok) return false;

    return unserializeViewPage(map["view"], pid);
}

bool GGViewProjectUnserializer::unserializeConnection(QVariant connection)
{
    bool ok = GGBasicProjectUnserializer::unserializeConnection(connection);
    if (!ok) return false;
    QVariantMap map;
    connection >> map;
    if (!map.contains("view")) return false;

    if (!m_processor->processConnectionRef(map["id"])) return false;
    GG::ConnectionID cid = static_cast<GG::ConnectionID> (map["id"].toUInt(&ok));
    if (!ok) return false;

    return unserializeViewConnection(map["view"], cid);
}

bool GGViewProjectUnserializer::unserializeForeignPage(QVariant page)
{
    if (!page.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    page >> map;
    if (!map.contains("viewPage") || !map.contains("pageId")) return false;

    bool ok = m_processor->processPageRef(map["pageId"]);
    if (!ok) return false;

    GG::PageID pid = static_cast<GG::PageID> (map["pageId"].toUInt(&ok));
    if (!ok) return false;

    QVariantList lst;
    map["viewPage"] >> lst;

    foreach (QVariant vp, lst) {
        ok &= unserializeViewPage(vp, pid);
    }
    if (!ok) return false;
    return true;
}

bool GGViewProjectUnserializer::unserializeForeignConnection(QVariant connection)
{
    if (!connection.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    connection >> map;
    if (!map.contains("viewConnection") || !map.contains("connectionId")) return false;

    bool ok = m_processor->processConnectionRef(map["connectionId"]);
    if (!ok) return false;

    GG::ConnectionID cid = static_cast<GG::ConnectionID> (map["connectionId"].toUInt(&ok));
    if (!ok) return false;

    QVariantList lst;
    map["viewConnection"] >> lst;

    foreach (QVariant vp, lst) {
        ok &= unserializeViewConnection(vp, cid);
    }
    if (!ok) return false;
    return true;
}

bool GGViewProjectUnserializer::finalizeUnserialization()
{
    bool ok = GGBasicProjectUnserializer::finalizeUnserialization();
    if (!ok) return false;

    // All Pages and Connections have been registered.

    // Inject view scenes
    foreach (GGScene *sc, m_project->editModel()->getScenes()) {
        GGViewScene *vs = new GGViewScene(sc, m_viewModel);
        if (!this->injectViewScene(m_viewModel, vs)) {
            delete vs;
            return false;
        }
    }

    // Now add viewpages
    foreach (GG::PageID pid, m_vpMap.keys()) {
        foreach (SceneRectPair srp, m_vpMap[pid]) {
            GGPage *p = m_project->editModel()->getPage(pid);
            if (!p) return false;
            GGViewPage *vp = new GGViewPage(p, m_viewModel, srp.second, srp.first);
            if (!this->injectViewPage(m_viewModel, vp)) {
                delete vp;
                return false;
            }
        }
    }

    // Now add viewconnections
    foreach (GG::ConnectionID cid, m_vcMap.keys()) {
        foreach (GG::SceneID sid, m_vcMap[cid]) {
            GGConnection *c = m_project->editModel()->getConnection(cid);
            if (!c) return false;
            GGViewConnection *vc = new GGViewConnection(c, m_viewModel, sid);
            if (!this->injectViewConnection(m_viewModel, vc)) {
                delete vc;
                return false;
            }
        }
    }

    return true;
}

bool GGViewProjectUnserializer::unserializeViewPage(QVariant viewPage, GG::PageID pid)
{
    if (!viewPage.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    viewPage >> map;

    if (!map.contains("scene") || !map.contains("rect")) return false;
    if (!m_processor->processSceneRef(map["scene"])) return false;

    bool ok = true;
    GG::SceneID sid = static_cast<GG::SceneID> (map["scene"].toUInt(&ok));
    if (!ok) return false;

    QRect r;
    map["rect"] >> r;

    m_vpMap[pid] << qMakePair(sid, r);
    return true;
}

bool GGViewProjectUnserializer::unserializeViewConnection(QVariant viewConnection, GG::ConnectionID cid)
{
    if (!viewConnection.canConvert<QVariantMap>()) return false;
    QVariantMap map;
    viewConnection >> map;

    if (!map.contains("scene")) return false;
    if (!m_processor->processSceneRef(map["scene"])) return false;

    bool ok = true;
    GG::SceneID sid = static_cast<GG::SceneID> (map["scene"].toUInt(&ok));
    if (!ok) return false;

    m_vcMap[cid] << sid;
    return true;
}
