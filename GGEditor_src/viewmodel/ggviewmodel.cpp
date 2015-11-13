#include "ggviewmodel.h"
#include <model/ggeditmodel.h>
#include "ggviewpage.h"
#include "ggviewconnection.h"

GGViewModel::GGViewModel(GGEditModel *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{
    Q_ASSERT(m_model);
    connect(m_model, SIGNAL(pageRegistered(GGPage*)), this, SLOT(regPage(GGPage*)), Qt::QueuedConnection);
    connect(m_model, SIGNAL(pageUnregistered(GG::PageID,GGPage*)), this, SLOT(unregPage(GG::PageID,GGPage*)), Qt::QueuedConnection);
    connect(m_model, SIGNAL(connectionRegistered(GGConnection*)), this, SLOT(regConn(GGConnection*)), Qt::QueuedConnection);
    connect(m_model, SIGNAL(connectionUnRegistered(GG::ConnectionID,GGConnection*)), this, SLOT(unregConn(GG::ConnectionID,GGConnection*)), Qt::QueuedConnection);
}

GGViewModel::~GGViewModel()
{
    qDeleteAll(m_pageMap.values());
    qDeleteAll(m_connectionMap.values());
    qDeleteAll(m_pageRec.values());
    qDeleteAll(m_connectionRec.values());
}

void GGViewModel::purgeRecycledPage(GGPage *page)
{
    delete m_pageRec.take(page);
}

void GGViewModel::purgeRecycledConnection(GGConnection *conn)
{
    delete m_connectionRec.take(conn);
}

void GGViewModel::notifyViewPageUpdate(GGViewPage *page)
{
    if (m_pageMap.contains(page->page())) {
        emit viewPageUpdated(page);
    }
}

void GGViewModel::regPage(GGPage *page)
{
    GGViewPage *vp = NULL;
    // Page known?
    if (vp = m_pageMap.value(page)) {
        // TODO: emit signal?
        emit viewPageRegistered(vp);
        return;
    }
    // Page recycled?
    if (vp = m_pageRec.take(page)) {
        m_pageMap[page] = vp;
        // TODO: emit signal?
        emit viewPageRegistered(vp);
        return;
    }

    // Unknown, create one
    vp = new GGViewPage(page, this, QRect());
    m_pageMap[page] = vp;
    emit viewPageRegistered(vp);
}

void GGViewModel::unregPage(GG::PageID, GGPage *page)
{
    // This requires that pages are not deleted
    // Move to recycler
    GGViewPage *vp = m_pageMap.take(page);
    if (vp) {
        m_pageRec[page] = vp;
        emit viewPageUnregistered(vp);
    } else {
        Q_ASSERT_X(false, "GGViewModel::unregPage", "Page not known");
    }
}

void GGViewModel::regConn(GGConnection *conn)
{
    GGViewConnection *vc = NULL;
    // Connection known?
    if (vc = m_connectionMap.value(conn)) {
        // TODO: emit signal?
        emit viewConnectionRegistered(vc);
        return;
    }
    // Connection recycled?
    if (vc = m_connectionRec.take(conn)) {
        m_connectionMap[conn] = vc;
        // TODO: emit signal?
        emit viewConnectionRegistered(vc);
    }

    // Unknown, create one
    vc = new GGViewConnection(conn, this);
    m_connectionMap[conn] = vc;
    emit viewConnectionRegistered(vc);
}

void GGViewModel::unregConn(GG::ConnectionID, GGConnection *conn)
{
    // This requires that connections are not deleted
    // Move to recycler
    GGViewConnection *vc = m_connectionMap.take(conn);
    if (vc) {
        m_connectionRec[conn] = vc;
        emit viewConnectionUnregistered(vc);
    } else {
        Q_ASSERT_X(false, "GGViewModel::unregConn", "Connection not known");
    }
}

void GGViewModel::updPage(GGPage *page)
{
    if (GGViewPage *vp = m_pageMap.value(page)) {
        emit pageUpdated(vp);
    } else {
        Q_ASSERT_X(false, "GGViewModel::updPage", "Page not known");
    }
}
