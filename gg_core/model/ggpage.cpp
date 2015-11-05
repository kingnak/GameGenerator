#include "ggpage.h"
#include "ggconnection.h"
#include <QList>

GGPage::GGPage()
:   m_id(GG::InvalidPageId),
    m_model(NULL)
{

}

GGPage::~GGPage()
{
}

/////////////////////////////////////////

GGConditionPage::GGConditionPage()
    : m_true(NULL), m_false(NULL)
{

}

int GGConditionPage::type() const
{
    return Type;
}

QList<GGConnection *> GGConditionPage::getConnections()
{
    QList<GGConnection *> ret;
    if (m_true) ret << m_true;
    if (m_false) ret << m_false;
    return ret;
}

bool GGConditionPage::removeConnection(GGConnection *connection)
{
    Q_ASSERT(connection && connection->source() == this);
    if (connection == m_true) {
        m_true = NULL;
        return true;
    }
    if (connection == m_false) {
        m_false = NULL;
        return true;
    }
    return false;
}
/////////////////////////////////////////

GGContentPage::GGContentPage()
    : m_content(NULL)
{

}

/////////////////////////////////////////

GGStartPage::GGStartPage()
    : m_conn(NULL)
{

}

int GGStartPage::type() const
{
    return Type;
}

void GGStartPage::setConnection(GGConnection *conn)
{
    m_conn = conn;
}

QList<GGConnection *> GGStartPage::getConnections()
{
    QList<GGConnection *> ret;
    if (m_conn)
        ret << m_conn;
    return ret;
}

bool GGStartPage::removeConnection(GGConnection *connection)
{
    Q_ASSERT(connection && connection->source() == this);
    if (connection == m_conn) {
        m_conn = NULL;
        return true;
    }
    return false;
}

/////////////////////////////////////////

GGEndPage::GGEndPage()
{

}

int GGEndPage::type() const
{
    return Type;
}

QList<GGConnection *> GGEndPage::getConnections()
{
    return QList<GGConnection *>();
}

bool GGEndPage::removeConnection(GGConnection *connection)
{
    Q_UNUSED(connection)
    return false;
}


