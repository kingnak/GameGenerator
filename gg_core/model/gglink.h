#ifndef GGLINK_H
#define GGLINK_H

#include <gg_core_global.h>
#include <QString>

class GGConnection;

class GG_CORESHARED_EXPORT GGLink
{
public:
    GGLink();

    QString name() const { return m_name; }
    GGConnection *connection() const { return m_connection; }

    void setName(QString n) { m_name = n; }
    void setConnection(GGConnection *conn) { m_connection = conn; }

    // TODO: Add action

protected:
    QString m_name;
    GGConnection *m_connection;

};

#endif // GGLINK_H
