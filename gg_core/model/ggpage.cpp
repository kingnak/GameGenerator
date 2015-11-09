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
    for (int i = 0; i < m_mappedLinks.size(); ++i) {
        if (m_mappedLinks[i].link().connection() == connection) {
            // Make a copy, remove connection, and replace
            GGLink l = m_mappedLinks[i].link();
            l.setConnection(NULL);
            m_mappedLinks[i].setLink(l);
            return true;
        }
    }
    return false;
}

QList<GGMappedLink> GGMappedContentPage::getLinkMap() const
{
    return m_mappedLinks;
}

QList<GGConnection *> GGMappedContentPage::getMappedConnections() const
{
    QList<GGConnection *> conns;
    foreach (const GGMappedLink &mc, m_mappedLinks) {
        if (mc.link().connection()) conns << mc.link().connection();
    }
    return conns;
}

void GGMappedContentPage::addMappedLink(GGMappedLink link)
{
    m_mappedLinks << link;
}

bool GGMappedContentPage::setMappedLink(int idx, GGMappedLink link)
{
    if (0 <= idx && idx <= m_mappedLinks.size()) {
        m_mappedLinks[idx] = link;
        return true;
    }
    return false;
}

bool GGMappedContentPage::removeMappedLink(int idx)
{
    if (0 <= idx && idx <= m_mappedLinks.size()) {
        m_mappedLinks.removeAt(idx);
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

GGLink GGActionPage::actionLink() const
{
    return m_actionLink;
}

void GGActionPage::setActionLink(GGLink link)
{
    m_actionLink = link;
}

bool GGActionPage::removeConnection(GGConnection *connection)
{
    if (m_actionLink.connection() == connection) {
        m_actionLink.setConnection(NULL);
        return true;
    }
    return GGMappedContentPage::removeConnection(connection);
}

QList<GGConnection *> GGActionPage::getConnections() const
{
    QList<GGConnection *> conns = this->getMappedConnections();
    if (m_actionLink.connection()) conns << m_actionLink.connection();
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
    QList<GGConnection *> conns;
    foreach (const GGLink &l, m_decisionLinks) {
        if (l.connection()) conns << l.connection();
    }
    return conns;
}

void GGDecisionPage::addDecisionLink(GGLink link)
{
    m_decisionLinks << link;
}

bool GGDecisionPage::setDecisionLink(int idx, GGLink link)
{
    if (0 <= idx && idx <= m_decisionLinks.size()) {
        m_decisionLinks[idx] = link;
        return true;
    }
    return false;
}

bool GGDecisionPage::removeDecisionLink(int idx)
{
    if (0 <= idx && idx <= m_decisionLinks.size()) {
        m_decisionLinks.removeAt(idx);
        return true;
    }
    return false;
}

QList<GGLink> GGDecisionPage::getDecisionLinks() const
{
    return m_decisionLinks;
}

QList<GGConnection *> GGDecisionPage::getConnections() const
{
    return getMappedConnections() + getDecisionConnections();
}

bool GGDecisionPage::removeConnection(GGConnection *connection)
{
    for (int i = 0; i < m_decisionLinks.size(); ++i) {
        if (m_decisionLinks[i].connection() == connection) {
            m_decisionLinks[i].setConnection(NULL);
            return true;
        }
    }
    return GGMappedContentPage::removeConnection(connection);
}
