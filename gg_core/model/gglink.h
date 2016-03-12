#ifndef GGLINK_H
#define GGLINK_H

#include <gg_core_global.h>
#include <QString>
#include <model/ggaction.h>
#include <model/ggsearch.h>

class GGConnection;

class GG_CORESHARED_EXPORT GGLink
{
public:
    GGLink();

    QString name() const { return m_name; }
    GGConnection *connection() const { return m_connection; }
    GGAction action() const;

    void setName(QString n) { m_name = n; }
    void setConnection(GGConnection *conn) { m_connection = conn; }
    void setAction(const GGAction &action);

    bool match(const GGSearchRequest &req, GGSearchResultList &results, GG::PageID pageId, GGConnectionSlotData asSlot) const;

protected:
    QString m_name;
    GGConnection *m_connection;
    GGAction m_action;
};

#endif // GGLINK_H
