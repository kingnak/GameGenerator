#ifndef GGCONNECTIONSLOT_H
#define GGCONNECTIONSLOT_H

#include <QList>
#include <model/ggconnectionslotdata.h>

class GGPage;
class GGConnection;
class GGEditModel;
class GGLink;

class GGConnectionSlot : public GGConnectionSlotData
{
public:
    GGConnectionSlot(SlotType type, int index = -1)
        : GGConnectionSlotData(type, index) {}
    GGConnectionSlot(GGConnectionSlotData data)
        : GGConnectionSlotData(data) {}

    GGConnection *getExistingConnection(const GGPage *page);
    GGLink getLink(const GGPage *page);

    bool connect(GGPage *page, GGConnection *conn, GGConnection **oldConnection = 0);
    bool canConnect(GGPage *page);

    static QList<GGConnectionSlot> enumerateConnections(const GGPage *page, SlotTypes types = AllConnections);

private:
    bool doConnectTest(bool doSet, GGPage *page, GGConnection *conn, GGConnection **oldConnection = 0, GGLink *outLink = 0);

};

#endif // GGCONNECTIONSLOT_H
