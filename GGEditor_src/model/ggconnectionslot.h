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
        NoConnection        = 0x0000,
        StartConnection     = 0x0001,
        TrueConnection      = 0x0002,
        FalseConnection     = 0x0004,
        MappedConnection    = 0x0008,
        ActionConnection    = 0x0010,
        DecisionConnection  = 0x0020,
        AllConnections      = 0x00FF
    };
    Q_DECLARE_FLAGS(SlotTypes, SlotType)

    GGConnectionSlot(SlotType type, int index = -1)
        : m_type(type), m_idx(index) {}

    SlotType type() const { return m_type; }
    int index() const { return m_idx; }
    bool isLink() const;

    bool connect(GGPage *page, GGConnection *conn, GGConnection **oldConnection = 0);
    GGConnection *getExistingConnection(GGPage *page);
    bool canConnect(GGPage *page);

    static GGConnectionSlot findConnection(const GGPage *page, const GGConnection *conn);
    static QList<GGConnectionSlot> enumerateConnections(const GGPage *page, SlotTypes types = AllConnections);

private:
    bool doConnectTest(bool doSet, GGPage *page, GGConnection *conn, GGConnection **oldConnection = 0);

private:
    SlotType m_type;
    int m_idx;
};

#endif // GGCONNECTIONSLOT_H
