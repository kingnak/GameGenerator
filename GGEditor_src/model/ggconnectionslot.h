#ifndef GGCONNECTIONSLOT_H
#define GGCONNECTIONSLOT_H

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

    GGConnectionSlot(SlotType type, int index = 0)
        : m_type(type), m_idx(index) {}

    SlotType type() const { return m_type; }
    int index() const { return m_idx; }

    bool apply(GGPage *page, GGConnection *conn);

    static GGConnectionSlot findConnection(const GGPage *page, const GGConnection *conn);

private:
    SlotType m_type;
    int m_idx;
};

#endif // GGCONNECTIONSLOT_H
