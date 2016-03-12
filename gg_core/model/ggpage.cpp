#include "ggpage.h"
#include "ggconnection.h"
#include "ggcontentelement.h"
#include "ggabstractmodel.h"
#include <QList>

GGPage::GGPage()
:   m_id(GG::InvalidPageId),
    m_model(NULL)
{

}

GGPage::~GGPage()
{
}

void GGPage::setSceneName(QString sn)
{
    if (sn != m_sceneName) {
        m_sceneName = sn;
        notifyChanged();
    }
}

void GGPage::setName(QString n)
{
    if (n != m_name) {
        m_name = n;
        notifyChanged();
    }
}

bool GGPage::match(const GGSearchRequest &req, GGSearchResultList &results) const
{
    bool res = false;
    if (req.what().testFlag(GGSearchRequest::PageName) && req.matches(m_name)) {
        results << GGSearchResult(GGSearchResult::PageName, GGSearchRequest::PageName, m_name, m_id);
        res = true;
    }
    if (req.what().testFlag(GGSearchRequest::PageScene) && req.matches(m_sceneName)) {
        results << GGSearchResult(GGSearchResult::PageScene, GGSearchRequest::PageScene, m_sceneName, m_id);
        res = true;
    }
    return res;
}

void GGPage::notifyChanged()
{
    if (m_model) m_model->notifyPageUpdate(m_id);
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
    if (t != m_true) {
        m_true = t;
        notifyChanged();
    }
}

void GGConditionPage::setFalseConnection(GGConnection *f)
{
    if (f != m_false) {
        m_false = f;
        notifyChanged();
    }
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
        notifyChanged();
        return true;
    }
    if (connection == m_false) {
        m_false = NULL;
        notifyChanged();
        return true;
    }
    return false;
}

bool GGConditionPage::match(const GGSearchRequest &req, GGSearchResultList &results) const
{
    bool res = GGPage::match(req, results);
    // TODO: Condition
    return res;
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

void GGContentPage::setCaption(QString cap)
{
    if (m_caption != cap) {
        m_caption = cap;
        notifyChanged();
    }
}

GGContentElement *GGContentPage::content()
{
    return m_content;
}

void GGContentPage::setContent(GGContentElement *cont)
{
    if (m_content != cont) {
        delete m_content;
        m_content = cont;
        notifyChanged();
    }
}

GGContentElement *GGContentPage::exchangeContent(GGContentElement *cont)
{
    if (m_content != cont) {
        GGContentElement *oldContent = m_content;
        m_content = cont;
        notifyChanged();
        return oldContent;
    }
    return cont;
}

bool GGContentPage::match(const GGSearchRequest &req, GGSearchResultList &results) const
{
    bool res = GGPage::match(req, results);
    if (req.what().testFlag(GGSearchRequest::PageCaption) && req.matches(m_caption)) {
        results << GGSearchResult(GGSearchResult::PageCaption, GGSearchRequest::PageCaption, m_caption, id());
        res = true;
    }
    // TODO: Content
    return res;
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
    if (conn != m_conn) {
        m_conn = conn;
        notifyChanged();
    }
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
        notifyChanged();
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
    Q_ASSERT(connection && connection->source() == this);
    for (int i = 0; i < m_mappedLinks.size(); ++i) {
        if (m_mappedLinks[i].link().connection() == connection) {
            // Make a copy, remove connection, and replace
            GGLink l = m_mappedLinks[i].link();
            l.setConnection(NULL);
            m_mappedLinks[i].setLink(l);
            notifyChanged();
            return true;
        }
    }
    return false;
}

QList<GGMappedLink> GGMappedContentPage::getLinkMap() const
{
    return m_mappedLinks;
}

bool GGMappedContentPage::match(const GGSearchRequest &req, GGSearchResultList &results) const
{
    bool res = GGContentPage::match(req, results);
    if (req.searchLinks()) {
        QList<GGConnectionSlotData> mapSlots = GGConnectionSlotData::enumerateConnections(this, GGConnectionSlotData::MappedConnection);
        foreach (GGConnectionSlotData d, mapSlots) {
            res |= m_mappedLinks[d.index()].link().match(req, results, id(), d);
        }
    }
    return res;
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
    notifyChanged();
}

bool GGMappedContentPage::insertMappedLink(int idx, GGMappedLink link)
{
    if (0 <= idx && idx <= m_mappedLinks.size()) {
        m_mappedLinks.insert(idx, link);
        notifyChanged();
        return true;
    }
    return false;
}

bool GGMappedContentPage::setMappedLink(int idx, GGMappedLink link)
{
    if (0 <= idx && idx < m_mappedLinks.size()) {
        m_mappedLinks[idx] = link;
        notifyChanged();
        return true;
    }
    return false;
}

bool GGMappedContentPage::removeMappedLink(int idx)
{
    if (0 <= idx && idx < m_mappedLinks.size()) {
        m_mappedLinks.removeAt(idx);
        notifyChanged();
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
    notifyChanged();
}

bool GGActionPage::removeConnection(GGConnection *connection)
{
    Q_ASSERT(connection && connection->source() == this);
    if (m_actionLink.connection() == connection) {
        m_actionLink.setConnection(NULL);
        notifyChanged();
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

bool GGActionPage::match(const GGSearchRequest &req, GGSearchResultList &results) const
{
    bool res = GGMappedContentPage::match(req, results);
    if (req.searchLinks()) {
        res |= m_actionLink.match(req, results, id(), GGConnectionSlotData(GGConnectionSlotData::ActionConnection));
    }
    return res;
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
    notifyChanged();
}

bool GGDecisionPage::insertDecisionLink(int idx, GGLink link)
{
    if (0 <= idx && idx <= m_decisionLinks.size()) {
        m_decisionLinks.insert(idx, link);
        notifyChanged();
        return true;
    }
    return false;
}

bool GGDecisionPage::setDecisionLink(int idx, GGLink link)
{
    if (0 <= idx && idx < m_decisionLinks.size()) {
        m_decisionLinks[idx] = link;
        notifyChanged();
        return true;
    }
    return false;
}

bool GGDecisionPage::removeDecisionLink(int idx)
{
    if (0 <= idx && idx < m_decisionLinks.size()) {
        m_decisionLinks.removeAt(idx);
        notifyChanged();
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
    Q_ASSERT(connection && connection->source() == this);
    for (int i = 0; i < m_decisionLinks.size(); ++i) {
        if (m_decisionLinks[i].connection() == connection) {
            m_decisionLinks[i].setConnection(NULL);
            notifyChanged();
            return true;
        }
    }
    return GGMappedContentPage::removeConnection(connection);
}

bool GGDecisionPage::match(const GGSearchRequest &req, GGSearchResultList &results) const
{
    bool res = GGMappedContentPage::match(req, results);
    if (req.searchLinks()) {
        QList<GGConnectionSlotData> decisionLinks = GGConnectionSlotData::enumerateConnections(this, GGConnectionSlotData::DecisionConnection);
        foreach (GGConnectionSlotData d, decisionLinks) {
            res |= m_decisionLinks[d.index()].match(req, results, id(), d);
        }
    }
    return res;
}
