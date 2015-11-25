#ifndef GGCONNECTIONSLOT_H
#define GGCONNECTIONSLOT_H

#include <QList>

class GGPage;
class GGConnection;
class GGEditModel;

class GGConnectionSlot
{
public:
    enum SlotType {
        NoConnection,
        StartConnection,
        TrueConnection, FalseConnection,
        MappedConnection,
        ActionConnection,
        DecisionConnection
    };

    GGConnectionSlot(SlotType type, int index = -1)
        : m_type(type), m_idx(index) {}

    SlotType type() const { return m_type; }
    int index() const { return m_idx; }

    bool connect(GGPage *page, GGConnection *conn, GGConnection **oldConnection = 0);
    GGConnection *getExistingConnection(GGPage *page);
    bool canConnect(GGPage *page);

    static GGConnectionSlot findConnection(const GGPage *page, const GGConnection *conn);
    static QList<GGConnectionSlot> enumerateConnections(const GGPage *page);

private:
    bool doConnectTest(bool doSet, GGPage *page, GGConnection *conn, GGConnection **oldConnection = 0);

private:
    SlotType m_type;
    int m_idx;
};

#endif // GGCONNECTIONSLOT_H
