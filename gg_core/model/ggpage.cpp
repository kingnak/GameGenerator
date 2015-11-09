#include "ggpage.h"
#include "ggconnection.h"
#include "ggcontentelement.h"
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

GGConnection *GGConditionPage::trueConnection() const
{
    return m_true;
}

GGConnection *GGConditionPage::falseConnection() const
{
    return m_false;
}

void GGConditionPage::setTrueConnection(GGConnection *t)
{
    m_true = t;
}

void GGConditionPage::setFalseConnection(GGConnection *f)
{
    m_false = f;
}

QList<GGConnection *> GGConditionPage::getConnections() const
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

GGContentPage::~GGContentPage()
{
    delete m_content;
}

GGContentElement *GGContentPage::content()
{
    return m_content;
}

void GGContentPage::setContent(GGContentElement *cont)
{
    if (m_content != cont) {
        delete m_content;
    }
    m_content = cont;
}

GGContentElement *GGContentPage::exchangeContent(GGContentElement *cont)
{
    GGContentElement *oldContent = m_content;
    m_content = cont;
    return oldContent;
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

void GGStartPage::setStartConnection(GGConnection *conn)
{
    m_conn = conn;
}

GGConnection *GGStartPage::startConnection() const
{
    return m_conn;
}

QList<GGConnection *> GGStartPage::getConnections() const
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

QList<GGConnection *> GGEndPage::getConnections() const
{
    return QList<GGConnection *>();
}

bool GGEndPage::removeConnection(GGConnection *connection)
{
    Q_UNUSED(connection)
    return false;
}

//////////////////////////////////////////

GGMappedContentPage::GGMappedContentPage()
{

}

bool GGMappedContentPage::removeConnection(GGConnection *connection)
{
    for (int i = 0; i < m_mappedConnections.size(); ++i) {
        if (m_mappedConnections[i].connection() == connection) {
            m_mappedConnections.removeAt(i);
            return true;
        }
    }
    return false;
}

QList<GGConnection *> GGMappedContentPage::getMappedConnections() const
{
    QList<GGConnection *> conns;
    foreach (GGMappedConnection mc, m_mappedConnections) {
        if (mc.connection()) conns << mc.connection();
    }
    return conns;
}

void GGMappedContentPage::addMappedConnection(GGMappedConnection conn)
{
    m_mappedConnections << conn;
}

bool GGMappedContentPage::setMappedConnection(int idx, GGMappedConnection conn)
{
    if (0 <= idx && idx <= m_mappedConnections.size()) {
        m_mappedConnections[idx] = conn;
        return true;
    }
    return false;
}

//////////////////////////////////////////

GGActionPage::GGActionPage()
{

}

int GGActionPage::type() const
{
    return Type;
}

bool GGActionPage::removeConnection(GGConnection *connection)
{
    // TODO: Check for action connection
    return GGMappedContentPage::removeConnection(connection);
}

QList<GGConnection *> GGActionPage::getConnections() const
{
    QList<GGConnection *> conns = this->getMappedConnections();
    // TODO: add action connection
    return conns;
}

//////////////////////////////////////////


GGDecisionPage::GGDecisionPage()
{

}

int GGDecisionPage::type() const
{
    return Type;
}

QList<GGConnection *> GGDecisionPage::getDecisionConnections() const
{
    return m_decisionConns;
}

void GGDecisionPage::addDecisionConnection(GGConnection *conn)
{
    // Allow null?
    m_decisionConns << conn;
}

bool GGDecisionPage::setDecisionConnection(int idx, GGConnection *conn)
{
    if (0 <= idx && idx <= m_decisionConns.size()) {
        // Allow null?
        m_decisionConns[idx] = conn;
        return true;
    }
    return false;
}

QList<GGConnection *> GGDecisionPage::getConnections() const
{
    return getMappedConnections() + getDecisionConnections();
}
