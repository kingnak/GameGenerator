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


