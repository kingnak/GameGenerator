#ifndef GGCONNECTION_H
#define GGCONNECTION_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>

class GGPage;
class GGAbstractModel;
class GGAbstractProjectUnserializer;

class GG_CORESHARED_EXPORT GGConnection
{
    Q_DISABLE_COPY(GGConnection)
public:
    GGConnection(GG::PageID srcId, GG::PageID destId);
    virtual ~GGConnection();

    GG::ConnectionID id() const { return m_id; }
    GG::PageID sourceId() const { return m_srcId; }
    GG::PageID destinationId() const { return m_destId; }
    GGPage *source() const { return m_src; }
    GGPage *destination() const { return m_dest; }

    GGAbstractModel *model() const { return m_model; }

private:
    // These are really private!
    // Only friend GGAbstractModel must touch these!
    GG::ConnectionID m_id;
    GG::PageID m_srcId;
    GG::PageID m_destId;
    GGPage *m_src;
    GGPage *m_dest;
    GGAbstractModel *m_model;

    friend class GGAbstractModel;
    friend class GGAbstractProjectUnserializer;
};

#endif // GGCONNECTION_H
