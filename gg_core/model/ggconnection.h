#ifndef GGCONNECTION_H
#define GGCONNECTION_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>

class GGPage;
class GGAbstractModel;

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
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGLink : public GGConnection
{
public:
    GGLink(GG::PageID srcId, GG::PageID destId) : GGConnection(srcId, destId) {}

    QString sceneName() const { return m_sceneName; }
    QString name() const { return m_name; }

    void setSceneName(QString sn) { m_sceneName = sn; }
    void setName(QString n) { m_name = n; }

    // TODO: Add action

protected:
    QString m_sceneName;
    QString m_name;

};

#endif // GGCONNECTION_H
