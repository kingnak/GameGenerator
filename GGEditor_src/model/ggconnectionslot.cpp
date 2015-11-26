#include "ggconnectionslot.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <QList>

bool GGConnectionSlot::doConnectTest(bool doSet, GGPage *page, GGConnection *conn, GGConnection **oldConnection)
{
    GGConnection *dummy_old = NULL;
    if (!oldConnection) oldConnection = &dummy_old;
    *oldConnection = NULL;

    switch (m_type) {
    case StartConnection:
        Q_ASSERT(ggpage_cast<GGStartPage*>(page));
        if (GGStartPage *sp = ggpage_cast<GGStartPage*>(page)) {
            *oldConnection = sp->startConnection();
            if (doSet) sp->setStartConnection(conn);
            return true;
        }
        return false;
    case TrueConnection:
        Q_ASSERT(ggpage_cast<GGConditionPage*>(page));
        if (GGConditionPage *cp = ggpage_cast<GGConditionPage*>(page)) {
            *oldConnection = cp->trueConnection();
            if (doSet) cp->setTrueConnection(conn);
            return true;
        }
        return false;
    case FalseConnection:
        Q_ASSERT(ggpage_cast<GGConditionPage*>(page));
        if (GGConditionPage *cp = ggpage_cast<GGConditionPage*>(page)) {
            *oldConnection = cp->falseConnection();
            if (doSet) cp->setFalseConnection(conn);
            return true;
        }
        return false;
    case MappedConnection:
        Q_ASSERT(GG::as<GGMappedContentPage>(page));
        Q_ASSERT(m_idx >= 0);

        { // Scope limit mcp
            GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(page);
            if (!mcp) return false;

            if (0 <= m_idx && m_idx < mcp->getLinkMap().size()) {
                // Must work on copies of Mapped Link and Link
                GGMappedLink mc = mcp->getLinkMap()[m_idx];
                GGLink l = mc.link();
                *oldConnection = l.connection();
                if (doSet) {
                    l.setConnection(conn);
                    mc.setLink(l);
                    mcp->setMappedLink(m_idx, mc);
                }
                return true;
            } else {
                return false;
            }
        }
        Q_ASSERT_X(false, "GGConnectionSlot::connect", "Can never reach this");
        return false;
    case ActionConnection:
        Q_ASSERT(ggpage_cast<GGActionPage*> (page));
        if (GGActionPage *ap = ggpage_cast<GGActionPage*> (page)) {
            GGLink l = ap->actionLink();
            *oldConnection = l.connection();
            if (doSet) {
                l.setConnection(conn);
                ap->setActionLink(l);
            }
            return true;
        }
        return false;
    case DecisionConnection:
        Q_ASSERT(ggpage_cast<GGDecisionPage*> (page));
        Q_ASSERT(m_idx >= 0);
        if (GGDecisionPage *dp = ggpage_cast<GGDecisionPage*> (page)) {
            if (0 <= m_idx && m_idx < dp->getDecisionLinks().size()) {
                // Must work on copy of Link
                GGLink l = dp->getDecisionLinks()[m_idx];
                *oldConnection = l.connection();
                if (doSet) {
                    l.setConnection(conn);
                    dp->setDecisionLink(m_idx, l);
                }
                return true;
            } else {
                return false;
            }
        }
        return false;
    case NoConnection:
        Q_ASSERT_X(false, "GGConnectionSlot::connect", "No Connection type");
        return false;
    default:
        Q_ASSERT_X(false, "GGConnectionSlot::connect", "Invalid Connection type");
        return false;
    }
}

bool GGConnectionSlot::isLink() const
{
    switch (m_type) {
    case StartConnection:
    case TrueConnection:
    case FalseConnection:
        return false;
    default:
        return true;
    }
}

bool GGConnectionSlot::connect(GGPage *page, GGConnection *conn, GGConnection **oldConnection)
{
    return doConnectTest(true, page, conn, oldConnection);
}

GGConnection *GGConnectionSlot::getExistingConnection(GGPage *page)
{
    GGConnection *ret = NULL;
    this->doConnectTest(false, page, NULL, &ret);
    return ret;
}

bool GGConnectionSlot::canConnect(GGPage *page)
{
    return doConnectTest(false, page, NULL);
}

GGConnectionSlot GGConnectionSlot::findConnection(const GGPage *page, const GGConnection *conn)
{
    if (!page || !conn) {
        return GGConnectionSlot(NoConnection);
    }

    // Const cast ok, as we don't access the value
    if (!page->getConnections().contains(const_cast<GGConnection *> (conn))) {
        return GGConnectionSlot(NoConnection);
    }

    if (const GGStartPage *sp = ggpage_cast<const GGStartPage*> (page)) {
        if (sp->startConnection() == conn) {
            return GGConnectionSlot(StartConnection);
        }
        Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "Connection is in list of connections, but page is a StartPage and it is not its start connection");
        return GGConnectionSlot(NoConnection);
    }

    if (ggpage_cast<const GGEndPage*> (page)) {
        Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "Connection is in list of connections, but page is a EndPage. EndPages don't have outgoing connections");
        return GGConnectionSlot(NoConnection);
    }

    if (const GGConditionPage *cp = ggpage_cast<const GGConditionPage*> (page)) {
        if (cp->trueConnection() == conn) {
            return GGConnectionSlot(TrueConnection);
        }
        if (cp->falseConnection() == conn) {
            return GGConnectionSlot(FalseConnection);
        }
        Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "Connection is neither true nor false connection for condition page");
        return GGConnectionSlot(NoConnection);
    }

    const GGActionPage *ap = ggpage_cast<const GGActionPage*> (page);
    const GGDecisionPage *dp = ggpage_cast<const GGDecisionPage*> (page);
    const GGMappedContentPage *mcp = ap ? static_cast<const GGMappedContentPage *> (ap) : dp;
    if (mcp) {
        // Const cast ok, as we don't access the value
        int idx = mcp->getMappedConnections().indexOf(const_cast<GGConnection *> (conn));
        if (idx >= 0) {
            return GGConnectionSlot(MappedConnection, idx);
        }
    }

    if (ap) {
        if (conn == ap->actionLink().connection()) {
            return GGConnectionSlot(ActionConnection);
        }
        Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "Connection is neither mapped nor action connection for action page");
        return GGConnectionSlot(NoConnection);
    }

    if (dp) {
        // Const cast ok, as we don't access the value
        int idx = dp->getDecisionConnections().indexOf(const_cast<GGConnection *> (conn));
        if (idx >= 0) {
            return GGConnectionSlot(DecisionConnection, idx);
        }
        Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "Connection is neither mapped nor decision connection for decision page");
        return GGConnectionSlot(NoConnection);
    }

    Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "Cannot determine connection slot");
    return GGConnectionSlot(NoConnection);
}

QList<GGConnectionSlot> GGConnectionSlot::enumerateConnections(const GGPage *page)
{
    QList<GGConnectionSlot> ret;
    if (GG::as<const GGStartPage>(page)) {
        ret << StartConnection;
    }
    if (GG::as<const GGConditionPage>(page)) {
        ret << TrueConnection << FalseConnection;
    }
    if (GG::as<const GGActionPage>(page)) {
        ret << ActionConnection;
    }
    if (const GGDecisionPage *dp = GG::as<const GGDecisionPage>(page)) {
        for (int i = 0; i < dp->getDecisionLinks().size(); ++i) {
            ret << GGConnectionSlot(DecisionConnection, i);
        }
    }
    if (const GGMappedContentPage *mcp = GG::as<const GGMappedContentPage>(page)) {
        for (int i = 0; i < mcp->getLinkMap().size(); ++i) {
            ret << GGConnectionSlot(MappedConnection, i);
        }
    }
    return ret;
}
