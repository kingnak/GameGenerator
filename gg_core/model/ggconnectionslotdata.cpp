#include "ggconnectionslotdata.h"
#include "ggpage.h"
#include "ggconnection.h"

bool GGConnectionSlotData::isLink() const
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

GGConnectionSlotData GGConnectionSlotData::findConnection(const GGPage *page, const GGConnection *conn)
{
    if (!page || !conn) {
        return GGConnectionSlotData(NoConnection);
    }

    // Const cast ok, as we don't access the value
    if (!page->getConnections().contains(const_cast<GGConnection *> (conn))) {
        return GGConnectionSlotData(NoConnection);
    }

    if (const GGStartPage *sp = ggpage_cast<const GGStartPage*> (page)) {
        if (sp->startConnection() == conn) {
            return GGConnectionSlotData(StartConnection);
        }
        Q_ASSERT_X(false, "GGConnectionSlotData::findConnection", "Connection is in list of connections, but page is a StartPage and it is not its start connection");
        return GGConnectionSlotData(NoConnection);
    }

    if (ggpage_cast<const GGEndPage*> (page)) {
        Q_ASSERT_X(false, "GGConnectionSlotData::findConnection", "Connection is in list of connections, but page is a EndPage. EndPages don't have outgoing connections");
        return GGConnectionSlotData(NoConnection);
    }

    if (const GGConditionPage *cp = ggpage_cast<const GGConditionPage*> (page)) {
        if (cp->trueConnection() == conn) {
            return GGConnectionSlotData(TrueConnection);
        }
        if (cp->falseConnection() == conn) {
            return GGConnectionSlotData(FalseConnection);
        }
        Q_ASSERT_X(false, "GGConnectionSlotData::findConnection", "Connection is neither true nor false connection for condition page");
        return GGConnectionSlotData(NoConnection);
    }

    const GGActionPage *ap = ggpage_cast<const GGActionPage*> (page);
    const GGDecisionPage *dp = ggpage_cast<const GGDecisionPage*> (page);
    const GGMappedContentPage *mcp = ap ? static_cast<const GGMappedContentPage *> (ap) : dp;
    if (mcp) {
        // Const cast ok, as we don't access the value
        int idx = mcp->getMappedConnections().indexOf(const_cast<GGConnection *> (conn));
        if (idx >= 0) {
            return GGConnectionSlotData(MappedConnection, idx);
        }
    }

    if (ap) {
        if (conn == ap->actionLink().connection()) {
            return GGConnectionSlotData(ActionConnection);
        }
        Q_ASSERT_X(false, "GGConnectionSlotData::findConnection", "Connection is neither mapped nor action connection for action page");
        return GGConnectionSlotData(NoConnection);
    }

    if (dp) {
        // Const cast ok, as we don't access the value
        int idx = dp->getDecisionConnections().indexOf(const_cast<GGConnection *> (conn));
        if (idx >= 0) {
            return GGConnectionSlotData(DecisionConnection, idx);
        }
        Q_ASSERT_X(false, "GGConnectionSlotData::findConnection", "Connection is neither mapped nor decision connection for decision page");
        return GGConnectionSlotData(NoConnection);
    }

    Q_ASSERT_X(false, "GGConnectionSlotData::findConnection", "Cannot determine connection slot");
    return GGConnectionSlotData(NoConnection);
}

QList<GGConnectionSlotData> GGConnectionSlotData::enumerateConnections(const GGPage *page, SlotTypes types)
{
    QList<GGConnectionSlotData> ret;
    if (types.testFlag(StartConnection) && GG::as<const GGStartPage>(page)) {
        ret << StartConnection;
    }
    if (GG::as<const GGConditionPage>(page)) {
        if (types.testFlag(TrueConnection))
            ret << TrueConnection;
        if (types.testFlag(FalseConnection))
            ret << FalseConnection;
    }
    if (types.testFlag(ActionConnection) && GG::as<const GGActionPage>(page)) {
        ret << ActionConnection;
    }
    if (types.testFlag(DecisionConnection)) {
        if (const GGDecisionPage *dp = GG::as<const GGDecisionPage>(page)) {
            for (int i = 0; i < dp->getDecisionLinks().size(); ++i) {
                ret << GGConnectionSlotData(DecisionConnection, i);
            }
        }
    }
    if (types.testFlag(MappedConnection)) {
        if (const GGMappedContentPage *mcp = GG::as<const GGMappedContentPage>(page)) {
            for (int i = 0; i < mcp->getLinkMap().size(); ++i) {
                ret << GGConnectionSlotData(MappedConnection, i);
            }
        }
    }
    return ret;
}
