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

QList<GGConnectionSlot> GGConnectionSlot::enumerateConnections(const GGPage *page, GGConnectionSlotData::SlotTypes types)
{
    QList<GGConnectionSlotData> sltsData = GGConnectionSlotData::enumerateConnections(page, types);
    QList<GGConnectionSlot> slts;
    foreach (GGConnectionSlotData d, sltsData) {
        slts << d;
    }
    return slts;
}

