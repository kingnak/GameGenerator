#include "ggmodelpagecommands.h"
#include "../model/ggeditmodel.h"
#include <model/ggabstractfactory.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggcontentelement.h>

GGCreatePageCmd::GGCreatePageCmd(GGEditModel *model, PageType type)
    : GGAbstractModelCommand(model), m_type(type), m_createdPage(NULL)
{

}

GGCreatePageCmd::~GGCreatePageCmd()
{
    if (m_state != Executed) {
        delete m_createdPage;
    }
}

bool GGCreatePageCmd::doExecute()
{
    switch (m_type) {
    case StartPage:
        m_createdPage = m_model->factory()->createStartPage();
        break;
    case EndPage:
        m_createdPage = m_model->factory()->createEndPage();
        break;
    case ConditionPage:
        m_createdPage = m_model->factory()->createConditionPage();
        break;
    case ActionPage:
        m_createdPage = m_model->factory()->createActionPage();
        break;
    case DecisionPage:
        m_createdPage = m_model->factory()->createDecisionPage();
        break;
    default:
        return setError("Unknown Page Type");
    }

    if (!m_model->registerNewPage(m_createdPage)) {
        delete m_createdPage;
        m_createdPage = NULL;
        return setError("Cannot register page");
    }

    return true;
}

bool GGCreatePageCmd::doUndo()
{
    // New page cannot have connections, but assert if not empty
    QList<GGConnection *> affected;
    if (!m_model->unregisterPage(m_createdPage->id(), &affected)) {
        return setError("Cannot unregister page");
    }
    Q_ASSERT_X(affected.isEmpty(), "GGCreatePageCmd::doUndo", "Created page has connections!");
    return true;
}

bool GGCreatePageCmd::doRedo()
{
    if (!m_model->registerPageWithId(m_createdPage)) {
        return setError("Cannot register page");
    }
    return true;
}

QString GGCreatePageCmd::description() const
{
    return "Create Page";
}

GGPage *GGCreatePageCmd::createdPage()
{
    return m_createdPage;
}


///////////////////////////

GGDeletePageCmd::GGDeletePageCmd(GGEditModel *model, GGPage *page)
    : GGAbstractModelCommand(model), m_deletedPage(page)
{
}

GGDeletePageCmd::~GGDeletePageCmd()
{
    if (m_state == Executed) {
        delete m_deletedPage;
        qDeleteAll(m_affectedConnections);
    }
}

QString GGDeletePageCmd::description() const
{
    return "Delete Page";
}

GGPage *GGDeletePageCmd::deletedPage()
{
    return m_deletedPage;
}

bool GGDeletePageCmd::doExecute()
{
    if (!m_deletedPage) {
        return setError("No page to delete");
    }

    if (m_model->getPage(m_deletedPage->id()) != m_deletedPage) {
        return setError("Page is not in model");
    }

    // Find source slots for all connections
    QList<GGConnection *> conns = m_model->getPageIncommingConnections(m_deletedPage);
    conns += m_deletedPage->getConnections();
    foreach (GGConnection *c, conns) {
        GGConnectionSlot s = GGConnectionSlot::findConnection(c->source(), c);
        Q_ASSERT(s.type() != GGConnectionSlot::NoConnection);
        PCS pcs = {s,c->source(),c};
        m_slots << pcs;
    }

    // Remove
    if (!m_model->unregisterPage(m_deletedPage->id(), &m_affectedConnections)) {
        return setError("Cannot delete page");
    }

    return true;
}

bool GGDeletePageCmd::doUndo()
{
    if (!m_deletedPage) {
        return setError("No page deleted");
    }

    if (!m_model->registerPageWithId(m_deletedPage)) {
        return setError("Cannot re-register page");
    }

    // Re-Register ALL connections
    foreach (GGConnection *c, m_affectedConnections) {
        if (!m_model->registerConnectionWithId(c)) {
            // Try to remove again
            if (!m_model->unregisterPage(m_deletedPage->id())) {
                Q_ASSERT_X(false, "GGDeletePageCmd::doUndo", "Re-Registering of connections failed, and unregistering page again failed, too");
            }
            return setError("Cannot re-register connections");
        }
    }

    // Re-Apply to slots
    foreach (PCS s, m_slots) {
        if (!s.slot.connect(s.page, s.conn)) {
            // Try to remove again
            if (!m_model->unregisterPage(m_deletedPage->id())) {
                Q_ASSERT_X(false, "GGDeletePageCmd::doUndo", "Re-Registering of connections to slots failed, and unregistering page again failed, too");
            }
            return setError("Cannot re-register connections");
        }
    }

    return true;
}

bool GGDeletePageCmd::doRedo()
{
    if (!m_model->unregisterPage(m_deletedPage->id())) {
        return setError("Cannot delete page");
    }

    return true;
}

///////////////////////////

GGSetPageStringCmd::GGSetPageStringCmd(GGEditModel *model, GGPage *page, QString str, GGSetPageStringCmd::Type type)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(str),
      m_type(type)
{

}

QString GGSetPageStringCmd::description() const
{
    return "Set " + cmdDesc();
}

bool GGSetPageStringCmd::doExecute()
{
    if (!doSet(m_new, &m_old)) {
        return setError("Cannot set" + cmdDesc());
    }
    return true;
}

bool GGSetPageStringCmd::doUndo()
{
    if (!doSet(m_old)) {
        return setError("Cannot set" + cmdDesc());
    }
    return true;
}

bool GGSetPageStringCmd::doRedo()
{
    if (!doSet(m_old)) {
        return setError("Cannot set" + cmdDesc());
    }
    return true;
}

bool GGSetPageStringCmd::doSet(QString v, QString *old)
{
    QString o;
    if (!old) old = &o;
    switch (m_type) {
    case Name:
        *old = m_page->name();
        m_page->setName(v);
        return true;
    case SceneName:
        *old = m_page->sceneName();
        m_page->setSceneName(v);
        return true;
    case Caption:
    {
        GGContentPage *c = ggpage_cast<GGActionPage*>(m_page);
        if (!c) c = ggpage_cast<GGDecisionPage*>(m_page);
        if (!c) return false;
        *old = c->caption();
        c->setCaption(v);
        return true;
    }
    default: return false;
    }
}

QString GGSetPageStringCmd::cmdDesc() const
{
    switch (m_type) {
    case Name: return "page name";
    case SceneName: return "page scene";
    case Caption: return "page caption";
    default: return "?";
    }
}

///////////////////////////

GGExchangeContentCmd::GGExchangeContentCmd(GGEditModel *model, GGContentPage *page, GGContentElement *newElement)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(newElement),
      m_old(NULL)
{

}

GGExchangeContentCmd::~GGExchangeContentCmd()
{
    if (state() != Undone) {
        delete m_old;
    } else {
        delete m_new;
    }
}

QString GGExchangeContentCmd::description() const
{
    return "Change content";
}

bool GGExchangeContentCmd::doExecute()
{
    m_old = m_page->content();
    m_page->setContent(m_new);
    return true;
}

bool GGExchangeContentCmd::doUndo()
{
    m_page->setContent(m_old);
    return true;
}

bool GGExchangeContentCmd::doRedo()
{
    m_page->setContent(m_new);
    return true;
}

///////////////////////////

GGSetActionLinkCmd::GGSetActionLinkCmd(GGEditModel *model, GGActionPage *page, GGLink lnk)
    : GGAbstractModelCommand(model), m_page(page), m_new(lnk)
{

}

QString GGSetActionLinkCmd::description() const
{
    return "Set Action Link";
}

bool GGSetActionLinkCmd::doExecute()
{
    Q_ASSERT_X(!m_new.connection() || m_model->getConnection(m_new.connection()->id()) == m_new.connection(), "GGSetActionLinkCmd::doExecute", "New Link's connection is not registered in model");

    m_old = m_page->actionLink();
    if (m_old.connection()) {
        bool ret = m_model->unregisterConnection(m_old.connection()->id());
        Q_ASSERT(ret);
        Q_UNUSED(ret);
    }
    m_page->setActionLink(m_new);
    return true;
}

bool GGSetActionLinkCmd::doUndo()
{
    if (m_new.connection()) {
        bool ret = m_model->unregisterConnection(m_new.connection()->id());
        Q_ASSERT(ret);
        Q_UNUSED(ret);
    }
    if (m_old.connection()) {
        bool ret = m_model->registerConnectionWithId(m_old.connection());
        Q_ASSERT(ret);
        Q_UNUSED(ret);
    }
    m_page->setActionLink(m_old);
    return true;
}

bool GGSetActionLinkCmd::doRedo()
{
    if (m_old.connection()) {
       bool ret = m_model->unregisterConnection(m_old.connection()->id());
       Q_ASSERT(ret);
       Q_UNUSED(ret);
    }
    if (m_new.connection()) {
        bool ret = m_model->registerConnectionWithId(m_new.connection());
        Q_ASSERT(ret);
        Q_UNUSED(ret);
    }
    m_page->setActionLink(m_new);
    return true;
}

///////////////////////////

GGMappedLinkCmd::GGMappedLinkCmd(GGEditModel *model, GGMappedContentPage *page, GGMappedLink lnk, GGMappedLinkCmd::Type type, int idx)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(lnk),
      m_type(type),
      m_idx(idx)
{

}

QString GGMappedLinkCmd::description() const
{
    switch (m_type) {
    case Add: return "Add Mapped Link";
    case Remove: return "Remove Mapped Link";
    case Set: return "Set Mapped Link";
    default: return "? Mapped Link";
    }
}

bool GGMappedLinkCmd::doExecute()
{
    if (m_type != Add) {
        Q_ASSERT(0 <= m_idx && m_idx < m_page->getLinkMap().size());
        m_old = m_page->getLinkMap().value(m_idx);
    }
    return doSet(m_new, m_old);
}

bool GGMappedLinkCmd::doUndo()
{
    return doSet(m_old, m_new);
}

bool GGMappedLinkCmd::doRedo()
{
    return doSet(m_new, m_old);
}

bool GGMappedLinkCmd::doSet(GGMappedLink n, GGMappedLink o)
{
    switch (m_type) {
    case Add:
        Q_ASSERT_X(!n.link().connection() || n.link().connection() == m_model->getConnection(n.link().connection()->id()), "GGMappedConnectionCmd::doSet Add", "New link's connection is not registered in the model");
        m_page->addMappedLink(n);
        return true;
    case Remove:
        if (!m_page->removeMappedLink(m_idx)) {
            return setError("Cannot remove mapped link");
        }
        if (o.link().connection()) {
            bool ret = m_model->unregisterConnection(o.link().connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;
    case Set:
        Q_ASSERT_X(!n.link().connection() || n.link().connection() == m_model->getConnection(n.link().connection()->id()), "GGMappedConnectionCmd::doSet Set", "New link's connection is not registered in the model");
        if (o.link().connection()) {
            bool ret = m_model->unregisterConnection(o.link().connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        if (!m_page->setMappedLink(m_idx, n)) {
            return setError("Cannot set mapped link");
        }
        if (n.link().connection()) {
            bool ret = m_model->registerConnectionWithId(n.link().connection());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;
    default:
        qFatal("GMappedConnectionCmd::doSet: Unknown Type");
        return false;
    }
}

///////////////////////////

GGDecisionLinkCmd::GGDecisionLinkCmd(GGEditModel *model, GGDecisionPage *page, GGLink lnk, Type type, int idx)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(lnk),
      m_type(type),
      m_idx(idx)
{

}

QString GGDecisionLinkCmd::description() const
{
    switch (m_type) {
    case Add: return "Add Decision Link";
    case Remove: return "Remove Decision Link";
    case Set: return "Set Decision Link";
    default: return "? Decision Link";
    }
}

bool GGDecisionLinkCmd::doExecute()
{
    if (m_type != Add) {
        Q_ASSERT(0 <= m_idx && m_idx < m_page->getDecisionLinks().size());
        m_old = m_page->getDecisionLinks().value(m_idx);
    }
    return doSet(m_new, m_old);
}

bool GGDecisionLinkCmd::doUndo()
{
    return doSet(m_old, m_new);
}

bool GGDecisionLinkCmd::doRedo()
{
    return doSet(m_new, m_old);
}

bool GGDecisionLinkCmd::doSet(GGLink n, GGLink o)
{
    switch (m_type) {
    case Add:
        Q_ASSERT_X(!n.connection() || n.connection() == m_model->getConnection(n.connection()->id()), "GGDecisionLinkCmd::doSet Add", "New link's connection is not registered in the model");
        m_page->addDecisionLink(n);
        return true;
    case Remove:
        if (!m_page->removeDecisionLink(m_idx)) {
            return setError("Cannot remove decision link");
        }
        if (o.connection()) {
            bool ret = m_model->unregisterConnection(o.connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;
    case Set:
        Q_ASSERT_X(!n.connection() || n.connection() == m_model->getConnection(n.connection()->id()), "GGDecisionLinkCmd::doSet Set", "New link's connection is not registered in the model");
        if (o.connection()) {
            bool ret = m_model->unregisterConnection(o.connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        if (!m_page->setDecisionLink(m_idx, n)) {
            return setError("Cannot set decision link");
        }
        if (n.connection()) {
            bool ret = m_model->registerConnectionWithId(n.connection());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;
    default:
        qFatal("GGDecisionLinkCmd::doSet: Unknown Type");
        return false;
    }
}
