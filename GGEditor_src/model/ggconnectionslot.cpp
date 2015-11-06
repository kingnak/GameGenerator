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
    case ActionConnection:
    case DecisionConnection:
        Q_ASSERT_X(false, "GGConnectionSlot::apply", "Connection type not yet implemented");
        return false;
    case NoConnection:
        Q_ASSERT_X(false, "GGConnectionSlot::apply", "No Connection type");
        return false;
    default:
        Q_ASSERT_X(false, "GGConnectionSlot::apply", "Invalid Connection type");
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
