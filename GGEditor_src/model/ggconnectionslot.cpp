#include "ggconnectionslot.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <QList>

bool GGConnectionSlot::connect(GGPage *page, GGConnection *conn, GGConnection **oldConnection)
{
    GGConnection *dummy_old = NULL;
    if (!oldConnection) oldConnection = &dummy_old;
    *oldConnection = NULL;

    switch (m_type) {
    case StartConnection:
        Q_ASSERT(ggpage_cast<GGStartPage*>(page));
        if (GGStartPage *sp = ggpage_cast<GGStartPage*>(page)) {
            *oldConnection = sp->startConnection();
            sp->setStartConnection(conn);
            return true;
        }
        return false;
    case TrueConnection:
        Q_ASSERT(ggpage_cast<GGConditionPage*>(page));
        if (GGConditionPage *cp = ggpage_cast<GGConditionPage*>(page)) {
            *oldConnection = cp->trueConnection();
            cp->setTrueConnection(conn);
            return true;
        }
        return false;
    case FalseConnection:
        Q_ASSERT(ggpage_cast<GGConditionPage*>(page));
        if (GGConditionPage *cp = ggpage_cast<GGConditionPage*>(page)) {
            *oldConnection = cp->falseConnection();
            cp->setFalseConnection(conn);
            return true;
        }
        return false;
    case MappedConnection:
        Q_ASSERT(ggpage_cast<GGActionPage*> (page) || ggpage_cast<GGDecisionPage*> (page));
        Q_ASSERT(m_idx >= 0);

        { // Scope limit mcp
            GGMappedContentPage *mcp = ggpage_cast<GGActionPage*> (page);
            if (!mcp) mcp = ggpage_cast<GGDecisionPage*> (page);
            if (!mcp) return false;

            if (m_idx < mcp->getLinkMap().size()) {
                // Must work on copies of Mapped Link and Link
                GGMappedLink mc = mcp->getLinkMap()[m_idx];
                GGLink l = mc.link();
                *oldConnection = l.connection();
                l.setConnection(conn);
                mc.setLink(l);
                mcp->setMappedLink(m_idx, mc);
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
            l.setConnection(conn);
            ap->setActionLink(l);
            return true;
        }
        return false;
    case DecisionConnection:
        Q_ASSERT(ggpage_cast<GGDecisionPage*> (page));
        Q_ASSERT(m_idx >= 0);
        if (GGDecisionPage *dp = ggpage_cast<GGDecisionPage*> (page)) {
            if (m_idx < dp->getDecisionConnections().size()) {
                // Must work on copy of Link
                GGLink l = dp->getDecisionLinks()[m_idx];
                *oldConnection = l.connection();
                l.setConnection(conn);
                dp->setDecisionLink(m_idx, l);
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
