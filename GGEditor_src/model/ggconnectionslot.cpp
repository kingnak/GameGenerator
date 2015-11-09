#include "ggconnectionslot.h"
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <QList>

bool GGConnectionSlot::connect(GGPage *page, GGConnection *conn)
{
    switch (m_type) {
    case StartConnection:
        Q_ASSERT(ggpage_cast<GGStartPage*>(page));
        ggpage_cast<GGStartPage*>(page)->setStartConnection(conn);
        return true;
    case TrueConnection:
        Q_ASSERT(ggpage_cast<GGConditionPage*>(page));
        ggpage_cast<GGConditionPage*>(page)->setTrueConnection(conn);
        return true;
    case FalseConnection:
        Q_ASSERT(ggpage_cast<GGConditionPage*>(page));
        ggpage_cast<GGConditionPage*>(page)->setFalseConnection(conn);
        return true;
    case MappedConnection:
        Q_ASSERT(ggpage_cast<GGActionPage*> (page) || ggpage_cast<GGDecisionPage*> (page));
        Q_ASSERT(m_idx >= 0);

        { // Scope limit mcp
            GGMappedContentPage *mcp = ggpage_cast<GGActionPage*> (page);
            if (!mcp) mcp = ggpage_cast<GGDecisionPage*> (page);
            if (m_idx < mcp->getConnectionMap().size()) {
                GGMappedConnection mc = mcp->getConnectionMap()[m_idx];
                mc.setConnection(conn);
                mcp->setMappedConnection(m_idx, mc);
                return true;
            } else {
                return false;
            }
        }
        Q_ASSERT_X(false, "GGConnectionSlot::connect", "Can never reach this");
        return false;
    case ActionConnection:
        Q_ASSERT(ggpage_cast<GGActionPage*> (page));
        // TODO: Set action connection
        Q_ASSERT_X(false, "GGConnectionSlot::connect", "Action connection not yet implemented");
        return true;
    case DecisionConnection:
        Q_ASSERT(ggpage_cast<GGDecisionPage*> (page));
        Q_ASSERT(m_idx >= 0);

        { // Scope limit dp
            GGDecisionPage *dp = ggpage_cast<GGDecisionPage*> (page);
            if (m_idx < dp->getDecisionConnections().size()) {
                dp->setDecisionConnection(m_idx, conn);
                return true;
            } else {
                return false;
            }
        }
        Q_ASSERT_X(false, "GGConnectionSlot::connect", "Can never reach this");
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

    Q_ASSERT_X(false, "GGConnectionSlot::findConnection", "TODO: Implement other page types");
    return GGConnectionSlot(NoConnection);
}
