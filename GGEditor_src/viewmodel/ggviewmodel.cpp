#include "ggviewmodel.h"
#include <model/ggeditmodel.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include "ggviewscene.h"
#include "ggviewpage.h"
#include "ggviewconnection.h"

GGViewModel::GGViewModel(GGEditModel *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{
    Q_ASSERT(m_model);
    qRegisterMetaType<GG::SceneID>("GG::SceneID");
    qRegisterMetaType<GG::PageID>("GG::PageID");
    qRegisterMetaType<GG::ConnectionID>("GG::ConnectionID");
    connect(m_model, SIGNAL(sceneRegistered(GGScene*)), this, SLOT(regScene(GGScene*)));
    connect(m_model, SIGNAL(sceneUnregistered(GG::SceneID,GGScene*)), this, SLOT(unregScene(GG::SceneID,GGScene*)));
    connect(m_model, SIGNAL(pageRegistered(GGPage*)), this, SLOT(regPage(GGPage*)));
    connect(m_model, SIGNAL(pageUnregistered(GG::PageID,GGPage*)), this, SLOT(unregPage(GG::PageID,GGPage*)));
    connect(m_model, SIGNAL(connectionRegistered(GGConnection*)), this, SLOT(regConn(GGConnection*)));
    connect(m_model, SIGNAL(connectionUnregistered(GG::ConnectionID,GGConnection*)), this, SLOT(unregConn(GG::ConnectionID,GGConnection*)));
    connect(m_model, SIGNAL(pageUpdated(GGPage*,GGAbstractModel::PageSections)), this, SLOT(updPage(GGPage*)));
}

GGViewModel::~GGViewModel()
{
    foreach (PageMap m, m_pageMap.values())
        qDeleteAll(m.values());
    foreach (ConnectionMap m, m_connectionMap.values())
        qDeleteAll(m.values());
    foreach (PageMap m, m_pageRec.values())
        qDeleteAll(m.values());
    foreach (ConnectionMap m, m_connectionRec.values())
        qDeleteAll(m.values());
}

GGViewScene *GGViewModel::getViewSceneForScene(GGScene *scene)
{
    return m_sceneMap.value(scene->id());
}

GGViewPage *GGViewModel::getViewPageForPage(GGPage *page, GG::SceneID scene)
{
    if (!m_pageMap.contains(scene)) {
        qWarning("Requesting ViewPage from unknown ViewScene");
    } else if (!m_pageMap.value(scene).contains(page->id())) {
        qWarning("Requesting ViewPage that is not known in this scene");
    }

    return m_pageMap.value(scene).value(page->id());
}

GGViewConnection *GGViewModel::getViewConnectionForConnection(GGConnection *conn, GG::SceneID scene)
{
    if (!m_connectionMap.contains(scene)) {
        qWarning("Requesting ViewConnection from unknown ViewScene");
    } else if (!m_connectionMap.value(scene).contains(conn->id())) {
        qWarning("Requesting ViewConnection that is not known in this scene");
    }

    return m_connectionMap.value(scene).value(conn->id());
}

QList<GGViewPage *> GGViewModel::getViewPagesInScene(GGScene *scene)
{
    return m_pageMap.value(scene->id()).values();
}

QList<GGViewConnection *> GGViewModel::getViewConnectionsInScene(GGScene *scene)
{
    return m_connectionMap.value(scene->id()).values();
}

//bool GGViewModel::registerPage(GGViewPage *page)
//{
//    Q_ASSERT(page);
//    Q_ASSERT(m_sceneMap.contains(page->page()->sceneId()));
//    Q_ASSERT(!m_pageMap[page->page()->sceneId()].contains(page->page()->id()));
//    if (m_pageMap[page->page()->sceneId()].contains(page->page()->id())) {
//        return false;
//    }
//    m_pageRec[page->page()->sceneId()].remove(page->page()->id());
//    m_pageMap[page->page()->sceneId()][page->page()->id()] = page;
//    return true;
//}

void GGViewModel::purgeRecycledPage(GGPage *page)
{
    for (QMap<GG::SceneID, PageMap>::iterator it = m_pageRec.begin(); it != m_pageRec.end(); ++it) {
        delete it.value().take(page->id());
    }
}

void GGViewModel::purgeRecycledConnection(GGConnection *conn)
{
    for (QMap<GG::SceneID, ConnectionMap>::iterator it = m_connectionRec.begin(); it != m_connectionRec.end(); ++it) {
        delete it.value().take(conn->id());
    }
}

void GGViewModel::notifyViewPageUpdate(GGViewPage *page)
{
    if (m_pageMap.value(page->viewSceneId()).contains(page->page()->id())) {
        emit viewPageUpdated(page);
    }
}

void GGViewModel::regScene(GGScene *scene)
{
    GGViewScene *vs = NULL;
    if ((vs = m_sceneMap.value(scene->id()))) {
        emit viewSceneRegistered(vs);
        return;
    }
    // We don't recycle view scenes now...
    vs = new GGViewScene(scene, this);
    m_sceneMap[scene->id()] = vs;
    emit viewSceneRegistered(vs);
}

void GGViewModel::unregScene(GG::SceneID id, GGScene *scene)
{
    Q_UNUSED(scene);
    GGViewScene *vs = m_sceneMap.take(id);
    if (vs) {
        Q_ASSERT(m_pageMap[id].isEmpty());
        Q_ASSERT(m_connectionMap[id].isEmpty());

        emit viewSceneUnregistered(vs);
        delete vs;
    } else {
        Q_ASSERT_X(false, "GGViewModel::unregScene", "Scene not known");
    }
}

void GGViewModel::regPage(GGPage *page)
{
    doRegPage(page, page->sceneId());
}

void GGViewModel::unregPage(GG::PageID id, GGPage *page)
{
    Q_UNUSED(page);
    // Move to recycler
    for (QMap<GG::SceneID, PageMap>::iterator it = m_pageMap.begin(); it != m_pageMap.end(); ++it) {
        GGViewPage *vp = m_pageMap[it.key()].take(id);
        if (vp) {
            m_pageRec[it.key()][id] = vp;
            emit viewPageUnregistered(vp);
        }
    }
}

void GGViewModel::regConn(GGConnection *conn)
{
    // Check for source and dest scene!
    if (conn->source()->sceneId() != conn->destination()->sceneId()) {
        // Register pages in foreign scenes, if not present yet
        if (!m_pageMap.value(conn->destination()->sceneId()).contains(conn->sourceId()))
            doRegPage(conn->source(), conn->destination()->sceneId());
        if (!m_pageMap.value(conn->source()->sceneId()).contains(conn->destinationId()))
            doRegPage(conn->destination(), conn->source()->sceneId());

        doRegConn(conn, conn->source()->sceneId());
        doRegConn(conn, conn->destination()->sceneId());
    } else {
        doRegConn(conn, conn->source()->sceneId());
    }
}

void GGViewModel::doRegConn(GGConnection *conn, GG::SceneID sceneId)
{
    GGViewConnection *vc = NULL;
    // Connection known?
    if ((vc = m_connectionMap.value(sceneId).value(conn->id()))) {
        // TODO: emit signal?
        emit viewConnectionRegistered(vc);
        return;
    }
    // Connection recycled?
    if ((vc = m_connectionRec[sceneId].take(conn->id()))) {
        m_connectionMap[sceneId][conn->id()] = vc;
        // TODO: emit signal?
        emit viewConnectionRegistered(vc);
        return;
    }

    // Unknown, create one
    vc = new GGViewConnection(conn, this, sceneId);
    m_connectionMap[sceneId][conn->id()] = vc;
    emit viewConnectionRegistered(vc);
}

void GGViewModel::unregConn(GG::ConnectionID id, GGConnection *conn)
{
    Q_UNUSED(id);
    // Remove connection
    doUnregConn(conn, conn->source()->sceneId());

    if (conn->source()->sceneId() != conn->destination()->sceneId()) {
        doUnregConn(conn, conn->destination()->sceneId());
        // It was a cross-scene connection.
        // Remove viewpages from foreign scenes, if there are no other connections to them
        if (!hasConnectionsInScene(conn->source(), conn->destination()->sceneId())) {
            unregViewPage(conn->source()->id(), conn->destination()->sceneId());
        }
        if (!hasConnectionsInScene(conn->destination(), conn->source()->sceneId())) {
            unregViewPage(conn->destination()->id(), conn->source()->sceneId());
        }
    }
}

void GGViewModel::doUnregConn(GGConnection *conn, GG::SceneID sceneId)
{
    GGViewConnection *vc = m_connectionMap[sceneId].take(conn->id());
    if (vc) {
        // Move to recycler
        m_connectionRec[sceneId][conn->id()] = vc;
        emit viewConnectionUnregistered(vc);
    }
}

bool GGViewModel::hasConnectionsInScene(GGPage *page, GG::SceneID scene)
{
    if (page->sceneId() == scene) return true;
    foreach (GGViewConnection *vp, m_connectionMap.value(scene)) {
        if (vp->connection()->source() == page) {
            return true;
        }
        if (vp->connection()->destination() == page) {
            return true;
        }
    }

    return false;
}

void GGViewModel::updPage(GGPage *page)
{
    // This will report page changes for EVERY view page!
    for (QMap<GG::SceneID, PageMap>::iterator it = m_pageMap.begin(); it != m_pageMap.end(); ++it) {
        if (GGViewPage *vp = it.value().value(page->id())) {
            emit pageUpdated(vp);
        }
    }
}

void GGViewModel::unregViewPage(GG::PageID pageId, GG::SceneID sceneId)
{
    GGViewPage *vp = m_pageMap[sceneId].take(pageId);
    if (vp) {
        m_pageRec[sceneId][pageId] = vp;
        emit viewPageUnregistered(vp);
    }
}

void GGViewModel::doRegPage(GGPage *page, GG::SceneID sceneId)
{
    GGViewPage *vp = NULL;
    // Page known?
    if ((vp = m_pageMap.value(sceneId).value(page->id()))) {
        // TODO: emit signal?
        emit viewPageRegistered(vp);
        return;
    }
    // Page recycled?
    if ((vp = m_pageRec[sceneId].take(page->id()))) {
        m_pageMap[sceneId][page->id()] = vp;
        // TODO: emit signal?
        emit viewPageRegistered(vp);
        return;
    }

    // Unknown, create one
    vp = new GGViewPage(page, this, QRect(), sceneId);
    m_pageMap[sceneId][page->id()] = vp;
    emit viewPageRegistered(vp);
}
