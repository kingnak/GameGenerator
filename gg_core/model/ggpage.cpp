#include "ggpage.h"
#include "ggconnection.h"
#include "ggcontentelement.h"
#include "ggabstractmodel.h"
#include "ggscene.h"
#include <QList>
#include <style/ggabstractstyler.h>

GGPage::GGPage(GG::SceneID scene)
:   m_id(GG::InvalidPageId),
    m_model(NULL),
    m_sceneId(scene),
    m_scene(NULL)
{

}

GGPage::~GGPage()
{
}

bool GGPage::setSceneId(GG::SceneID sceneId)
{
    Q_ASSERT(!m_model);
    Q_ASSERT(!m_scene);
    if (m_model || m_scene) {
        return false;
    }
    m_sceneId = sceneId;
    return true;
}

void GGPage::setName(QString n)
{
    if (n != m_name) {
        m_name = n;
        notifyChanged(GGAbstractModel::PageData);
    }
}

bool GGPage::match(const GGSearchRequest &req, GGSearchResult &results) const
{
    bool res = false;
    if (req.what().testFlag(GGSearchRequest::PageName) && req.matches(m_name)) {
        results << GGSearchResultItem(GGSearchResultItem::PageName, GGSearchRequest::PageName, m_name, m_id);
        res = true;
    }
//    if (req.what().testFlag(GGSearchRequest::PageScene) && req.matches(m_sceneName)) {
//        results << GGSearchResultItem(GGSearchResultItem::PageScene, GGSearchRequest::PageScene, m_sceneName, m_id);
//        res = true;
//    }
    return res;
}

void GGPage::notifyChanged(int sections)
{
    if (m_model) m_model->notifyPageUpdate(m_id, static_cast<GGAbstractModel::PageSections>(sections));
}

/////////////////////////////////////////

GGConditionPage::GGConditionPage(GG::SceneID scene)
    : GGPage(scene), m_true(NULL), m_false(NULL)
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
        notifyChanged(GGAbstractModel::Connections);
    }
}

void GGConditionPage::setFalseConnection(GGConnection *f)
{
    if (f != m_false) {
        m_false = f;
        notifyChanged(GGAbstractModel::Connections);
    }
}

GGCondition GGConditionPage::getCondition() const
{
    return m_cond;
}

void GGConditionPage::setCondition(const GGCondition &cond)
{
    m_cond = cond;
    notifyChanged(GGAbstractModel::Condition);
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
        notifyChanged(GGAbstractModel::Connections);
        return true;
    }
    if (connection == m_false) {
        m_false = NULL;
        notifyChanged(GGAbstractModel::Connections);
        return true;
    }
    return false;
}

bool GGConditionPage::match(const GGSearchRequest &req, GGSearchResult &results) const
{
    bool res = GGPage::match(req, results);

    if (m_cond.isValid()) {
        if (req.what().testFlag(GGSearchRequest::Variable) && req.matches(m_cond.variableName())) {
            results << GGSearchResultItem(GGSearchResultItem::Condition, GGSearchRequest::Variable, m_cond.toString(), id());
        }
    }
    return res;
}
/////////////////////////////////////////

GGContentPage::GGContentPage(GG::SceneID scene)
    : GGPage(scene), m_content(NULL)
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
        notifyChanged(GGAbstractModel::Caption);
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
        notifyChanged(GGAbstractModel::Content);
    }
}

GGContentElement *GGContentPage::exchangeContent(GGContentElement *cont)
{
    if (m_content != cont) {
        GGContentElement *oldContent = m_content;
        m_content = cont;
        notifyChanged(GGAbstractModel::Content);
        return oldContent;
    }
    return cont;
}

bool GGContentPage::match(const GGSearchRequest &req, GGSearchResult &results) const
{
    bool res = GGPage::match(req, results);
    if (req.what().testFlag(GGSearchRequest::PageCaption)) {
        QString capPlain = model()->getStyler()->plainText(m_caption);
        if (req.matches(capPlain)) {
            results << GGSearchResultItem(GGSearchResultItem::PageCaption, GGSearchRequest::PageCaption, capPlain, id());
            res = true;
        }
    }
    // TODO: Content
    return res;
}

/////////////////////////////////////////

GGStartPage::GGStartPage(GG::SceneID scene)
    : GGContentPage(scene), m_conn(NULL)
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
        notifyChanged(GGAbstractModel::Connections);
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
        notifyChanged(GGAbstractModel::Connections);
        return true;
    }
    return false;
}

/////////////////////////////////////////

GGEntryActionPage::GGEntryActionPage(GG::SceneID scene)
    : GGContentPage(scene)
{

}

GGEntryActionPage::~GGEntryActionPage()
{

}

void GGEntryActionPage::setEntryAction(GGAction action)
{
    m_entryAction = action;
    notifyChanged(GGAbstractModel::EntryAction);
}

bool GGEntryActionPage::match(const GGSearchRequest &req, GGSearchResult &results) const
{
    bool res = GGContentPage::match(req, results);
    if (req.what().testFlag(GGSearchRequest::Variable) && req.matches(m_entryAction.variableName())) {
        results << GGSearchResultItem(GGSearchResultItem::EntryAction, GGSearchRequest::Variable, m_entryAction.toString(), id());
        res = true;
    }
    return res;
}

/////////////////////////////////////////

GGEndPage::GGEndPage(GG::SceneID scene)
    : GGEntryActionPage(scene)
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

GGMappedContentPage::GGMappedContentPage(GG::SceneID scene)
    : GGEntryActionPage(scene)
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
            notifyChanged(GGAbstractModel::Connections);
            return true;
        }
    }
    return false;
}

QList<GGMappedLink> GGMappedContentPage::getLinkMap() const
{
    return m_mappedLinks;
}

bool GGMappedContentPage::match(const GGSearchRequest &req, GGSearchResult &results) const
{
    bool res = GGEntryActionPage::match(req, results);
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
    notifyChanged(GGAbstractModel::MappedLinks);
}

bool GGMappedContentPage::insertMappedLink(int idx, GGMappedLink link)
{
    if (0 <= idx && idx <= m_mappedLinks.size()) {
        m_mappedLinks.insert(idx, link);
        notifyChanged(GGAbstractModel::MappedLinks);
        return true;
    }
    return false;
}

bool GGMappedContentPage::setMappedLink(int idx, GGMappedLink link)
{
    if (0 <= idx && idx < m_mappedLinks.size()) {
        m_mappedLinks[idx] = link;
        notifyChanged(GGAbstractModel::MappedLinks);
        return true;
    }
    return false;
}

bool GGMappedContentPage::removeMappedLink(int idx)
{
    if (0 <= idx && idx < m_mappedLinks.size()) {
        m_mappedLinks.removeAt(idx);
        notifyChanged(GGAbstractModel::MappedLinks);
        return true;
    }
    return false;
}

//////////////////////////////////////////

GGActionPage::GGActionPage(GG::SceneID scene)
    : GGMappedContentPage(scene)
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
    notifyChanged(GGAbstractModel::PageData | GGAbstractModel::Connections);
}

bool GGActionPage::removeConnection(GGConnection *connection)
{
    Q_ASSERT(connection && connection->source() == this);
    if (m_actionLink.connection() == connection) {
        m_actionLink.setConnection(NULL);
        notifyChanged(GGAbstractModel::Connections);
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

bool GGActionPage::match(const GGSearchRequest &req, GGSearchResult &results) const
{
    bool res = GGMappedContentPage::match(req, results);
    if (req.searchLinks()) {
        res |= m_actionLink.match(req, results, id(), GGConnectionSlotData(GGConnectionSlotData::ActionConnection));
    }
    return res;
}

//////////////////////////////////////////


GGDecisionPage::GGDecisionPage(GG::SceneID scene)
    : GGMappedContentPage(scene)
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
    notifyChanged(GGAbstractModel::DecisionLinks);
}

bool GGDecisionPage::insertDecisionLink(int idx, GGLink link)
{
    if (0 <= idx && idx <= m_decisionLinks.size()) {
        m_decisionLinks.insert(idx, link);
        notifyChanged(GGAbstractModel::DecisionLinks);
        return true;
    }
    return false;
}

bool GGDecisionPage::setDecisionLink(int idx, GGLink link)
{
    if (0 <= idx && idx < m_decisionLinks.size()) {
        m_decisionLinks[idx] = link;
        notifyChanged(GGAbstractModel::DecisionLinks);
        return true;
    }
    return false;
}

bool GGDecisionPage::removeDecisionLink(int idx)
{
    if (0 <= idx && idx < m_decisionLinks.size()) {
        m_decisionLinks.removeAt(idx);
        notifyChanged(GGAbstractModel::DecisionLinks);
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
            notifyChanged(GGAbstractModel::Connections);
            return true;
        }
    }
    return GGMappedContentPage::removeConnection(connection);
}

bool GGDecisionPage::match(const GGSearchRequest &req, GGSearchResult &results) const
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
