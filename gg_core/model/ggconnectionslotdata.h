#ifndef GGCONNECTIONSLOTDATA_H
#define GGCONNECTIONSLOTDATA_H

#include <gg_core_global.h>

class GGPage;
class GGConnection;

class GG_CORESHARED_EXPORT GGConnectionSlotData
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

    GGConnectionSlotData()
        : m_type(NoConnection), m_idx(-1) {}
    GGConnectionSlotData(SlotType type, int index = -1)
        : m_type(type), m_idx(index) {}

    SlotType type() const { return m_type; }
    int index() const { return m_idx; }

    bool isLink() const;

    static GGConnectionSlotData findConnection(const GGPage *page, const GGConnection *conn);
    static QList<GGConnectionSlotData> enumerateConnections(const GGPage *page, SlotTypes types = AllConnections);

protected:
    SlotType m_type;
    int m_idx;
};

#endif // GGCONNECTIONSLOTDATA_H
