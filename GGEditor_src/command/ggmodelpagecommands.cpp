#include "ggmodelpagecommands.h"
#include "../model/ggeditmodel.h"
#include <model/ggabstractfactory.h>
#include <model/ggscene.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>
#include <model/ggcontentelement.h>

GGCreateSceneCmd::GGCreateSceneCmd(GGEditModel *model, const QString &name, const QString &mediaDir)
    : GGAbstractModelCommand(model),
      m_createdScene(NULL),
      m_name(name),
      m_dir(mediaDir)
{

}

GGCreateSceneCmd::~GGCreateSceneCmd()
{
    if (state() != Executed) {
        delete m_createdScene;
    }
}

QString GGCreateSceneCmd::description() const
{
    return "Create Scene";
}

GGScene *GGCreateSceneCmd::createdScene()
{
    return m_createdScene;
}

bool GGCreateSceneCmd::doExecute()
{
    m_createdScene = m_model->factory()->createScene();
    m_createdScene->setName(m_name);
    if (m_dir.isEmpty())
        m_createdScene->setMediaDir(m_name);
    else
        m_createdScene->setMediaDir(m_dir);

    if (!m_model->registerNewScene(m_createdScene)) {
        delete m_createdScene;
        m_createdScene = NULL;
        return setError("Cannot register scene");
    }
    return true;
}

bool GGCreateSceneCmd::doUndo()
{
    return m_model->unregisterScene(m_createdScene->id());
}

bool GGCreateSceneCmd::doRedo()
{
    return m_model->registerSceneWithId(m_createdScene);
}

///////////////////////////

GGRenameSceneCmd::GGRenameSceneCmd(GGEditModel *model, GGScene *scene, const QString &newName)
    : GGAbstractModelCommand(model),
      m_scene(scene),
      m_newName(newName)
{

}

GGRenameSceneCmd::~GGRenameSceneCmd()
{

}

QString GGRenameSceneCmd::description() const
{
    return "Rename scene";
}

bool GGRenameSceneCmd::doExecute()
{
    m_oldName = m_scene->name();
    m_scene->setName(m_newName);
    return true;
}

bool GGRenameSceneCmd::doUndo()
{
    m_scene->setName(m_oldName);
    return true;
}

bool GGRenameSceneCmd::doRedo()
{
    m_scene->setName(m_newName);
    return true;
}

///////////////////////////

GGRenameSceneMediaDirCmd::GGRenameSceneMediaDirCmd(GGEditModel *model, GGScene *scene, const QString &newName)
    : GGAbstractModelCommand(model),
      m_scene(scene),
      m_newName(newName)
{

}

GGRenameSceneMediaDirCmd::~GGRenameSceneMediaDirCmd()
{

}

QString GGRenameSceneMediaDirCmd::description() const
{
    return "Rename scene media directory";
}

bool GGRenameSceneMediaDirCmd::doExecute()
{
    m_oldName = m_scene->mediaDir();
    m_scene->setMediaDir(m_newName);
    return true;
}

bool GGRenameSceneMediaDirCmd::doUndo()
{
    m_scene->setMediaDir(m_oldName);
    return true;
}

bool GGRenameSceneMediaDirCmd::doRedo()
{
    m_scene->setMediaDir(m_newName);
    return true;
}

///////////////////////////

GGDeleteSceneCmd::GGDeleteSceneCmd(GGEditModel *model, GGScene *scene)
    : GGAbstractModelCommand(model),
      m_deletedScene(scene)
{

}

GGDeleteSceneCmd::~GGDeleteSceneCmd()
{
    if (state() == Executed) {
        delete m_deletedScene;
    }
}

QString GGDeleteSceneCmd::description() const
{
    return "Delete Scene";
}

GGScene *GGDeleteSceneCmd::deletedScene()
{
    return m_deletedScene;
}

bool GGDeleteSceneCmd::doExecute()
{
    if (!m_deletedScene) {
        return setError("No scene set");
    }
    if (m_deletedScene->model() != m_model) {
        return setError("Scene is not in this model");
    }
    if (!m_model->unregisterScene(m_deletedScene->id())) {
        if (!m_deletedScene->pages().isEmpty()) {
            return setError("Scene is not empty");
        } else {
            return setError("Error deleting scene");
        }
    }
    return true;
}

bool GGDeleteSceneCmd::doUndo()
{
    return m_model->registerSceneWithId(m_deletedScene);
}

bool GGDeleteSceneCmd::doRedo()
{
    return m_model->unregisterScene(m_deletedScene->id());
}

///////////////////////////

GGCreatePageCmd::GGCreatePageCmd(GGEditModel *model, GGScene *scene, PageType type)
    : GGAbstractModelCommand(model),
      m_scene(scene),
      m_type(type),
      m_createdPage(NULL)
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
        m_createdPage = m_model->factory()->createStartPage(m_scene->id());
        break;
    case EndPage:
        m_createdPage = m_model->factory()->createEndPage(m_scene->id());
        break;
    case ConditionPage:
        m_createdPage = m_model->factory()->createConditionPage(m_scene->id());
        break;
    case ActionPage:
        m_createdPage = m_model->factory()->createActionPage(m_scene->id());
        break;
    case DecisionPage:
        m_createdPage = m_model->factory()->createDecisionPage(m_scene->id());
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

GGSetPageStringCmd::GGSetPageStringCmd(GGEditModel *model, GGPage *page, const QString &str, GGSetPageStringCmd::Type type)
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
    if (!doSet(m_new)) {
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
//    case SceneName:
//        *old = m_page->sceneName();
//        m_page->setSceneName(v);
//        return true;
    case Caption:
    {
        GGContentPage *c = GG::as<GGContentPage>(m_page);
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
//    case SceneName: return "page scene";
    case Caption: return "page caption";
    default: return "?";
    }
}

///////////////////////////

GGExchangeConditionCmd::GGExchangeConditionCmd(GGEditModel *model, GGConditionPage *page, const GGCondition &newCond)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(newCond)
{

}

GGExchangeConditionCmd::~GGExchangeConditionCmd()
{

}

QString GGExchangeConditionCmd::description() const
{
    return "Change Condition";
}

bool GGExchangeConditionCmd::doExecute()
{
    m_old = m_page->getCondition();
    return doRedo();
}

bool GGExchangeConditionCmd::doUndo()
{
    m_page->setCondition(m_old);
    return true;
}

bool GGExchangeConditionCmd::doRedo()
{
    m_page->setCondition(m_new);
    return true;
}

///////////////////////////

GGExchangeEntryActionCmd::GGExchangeEntryActionCmd(GGEditModel *model, GGEntryActionPage *page, const GGAction &newAction)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(newAction)
{

}

GGExchangeEntryActionCmd::~GGExchangeEntryActionCmd()
{

}

QString GGExchangeEntryActionCmd::description() const
{
    return "Update entry action";
}

bool GGExchangeEntryActionCmd::doExecute()
{
    m_old = m_page->entryAction();
    m_page->setEntryAction(m_new);
    return true;
}

bool GGExchangeEntryActionCmd::doUndo()
{
    m_page->setEntryAction(m_old);
    return true;
}

bool GGExchangeEntryActionCmd::doRedo()
{
    m_page->setEntryAction(m_new);
    return true;
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
    if (m_old != m_new) {
        if (state() != Undone) {
            delete m_old;
        } else {
            delete m_new;
        }
    }
}

QString GGExchangeContentCmd::description() const
{
    return "Change content";
}

bool GGExchangeContentCmd::doExecute()
{
    m_old = m_page->exchangeContent(m_new);
    return true;
}

bool GGExchangeContentCmd::doUndo()
{
    m_page->exchangeContent(m_old);
    return true;
}

bool GGExchangeContentCmd::doRedo()
{
    m_page->exchangeContent(m_new);
    return true;
}

///////////////////////////

GGSetActionLinkCmd::GGSetActionLinkCmd(GGEditModel *model, GGActionPage *page, const GGLink &lnk)
    : GGAbstractModelCommand(model), m_page(page), m_new(lnk)
{

}

QString GGSetActionLinkCmd::description() const
{
    return "Set Action Link";
}

bool GGSetActionLinkCmd::doExecute()
{
    Q_ASSERT_X(!m_new.connection(), "GGSetActionLinkCmd::doExecute", "New Link has a connection");
    if (m_new.connection()) {
        return setError("New Link has a connection");
    }
    m_old = m_page->actionLink();

    return doRedo();
}

bool GGSetActionLinkCmd::doUndo()
{
    m_old.setConnection(m_new.connection());
    m_new.setConnection(NULL);
    m_page->setActionLink(m_old);
    return true;
}

bool GGSetActionLinkCmd::doRedo()
{
    m_new.setConnection(m_old.connection());
    m_old.setConnection(NULL);
    m_page->setActionLink(m_new);
    return true;
}

///////////////////////////

GGMappedLinkCmd::GGMappedLinkCmd(GGEditModel *model, GGMappedContentPage *page, const GGMappedLink &lnk, GGMappedLinkCmd::Type type, int idx)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(lnk),
      m_type(type),
      m_idx(idx)
{

}

GGMappedLinkCmd::~GGMappedLinkCmd()
{
    if (m_type == Remove) {
        if (m_state == Executed) {
            delete m_old.link().connection();
        }
    }
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
        Q_ASSERT_X(!m_new.link().connection() || m_new.link().connection() == m_old.link().connection(), "GGMappedLinkCmd::doExecute", "New link has a connection");
        if (m_new.link().connection() && m_new.link().connection() != m_old.link().connection()) {
            return setError("New link has a connection");
        }
        // Same as Redo
        return doRedo();
    } else {
        Q_ASSERT_X(!m_new.link().connection(), "GGMappedLinkCmd::doExecute", "New link has a connection");
        if (m_new.link().connection()) {
            return setError("New link has a connection");
        }
        m_idx = m_page->getLinkMap().size();
        m_page->addMappedLink(m_new);
        return true;
    }
}

bool GGMappedLinkCmd::doUndo()
{
    switch (m_type) {
    case Add:
        if (!m_page->removeMappedLink(m_idx)) {
            return setError("Cannot remove mapped link");
        }
        Q_ASSERT(!m_new.link().connection());
        /*
         * New links never have connections
        if (m_new.link().connection()) {
            bool ret = m_model->unregisterConnection(m_new.link().connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        */
        return true;

    case Remove:
        if (!m_page->insertMappedLink(m_idx, m_old)) {
            return setError("Cannot insert mapped link");
        }
        if (m_old.link().connection()) {
            bool ret = m_model->registerConnectionWithId(m_old.link().connection());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;

    case Set:
    {
        // Connection is unaffected. Simply copy back to old
        GGLink ol = m_old.link();
        GGLink nl = m_new.link();
        ol.setConnection(nl.connection());
        m_old.setLink(ol);
        if (!m_page->setMappedLink(m_idx, m_old)) {
            // Unset in old
            ol.setConnection(NULL);
            m_old.setLink(ol);
            return setError("Cannot set mapped link");
        } else {
            // Unset in new
            nl.setConnection(NULL);
            m_new.setLink(nl);
            return true;
        }
    }

    default:
        Q_ASSERT(false);
        qFatal("Unknown DecisionLinkCmd Type");
        return false;
    }
}

bool GGMappedLinkCmd::doRedo()
{
    switch (m_type) {
    case Add:
        /*
         * New Links never have connection
        if (m_new.link().connection()) {
            if (!m_model->registerConnectionWithId(m_new.link().connection())) {
                Q_ASSERT(false);
                return setError("Cannot register connection");
            }
        }
        */
        Q_ASSERT(!m_new.link().connection());
        m_page->addMappedLink(m_new);
        return true;

    case Remove:
        if (!m_page->removeMappedLink(m_idx)) {
            return setError("Cannot remove mapped link");
        }
        if (m_old.link().connection()) {
            bool ret = m_model->unregisterConnection(m_old.link().connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;

    case Set:
    {
        // Connection is unaffected. Simply copy again to new
        GGLink ol = m_old.link();
        GGLink nl = m_new.link();
        nl.setConnection(ol.connection());
        m_new.setLink(nl);
        if (!m_page->setMappedLink(m_idx, m_new)) {
            // Unset in new
            nl.setConnection(NULL);
            m_new.setLink(nl);
            return setError("Cannot set decision link");
        } else {
            // Remove from old
            ol.setConnection(NULL);
            m_old.setLink(ol);
            return true;
        }
    }
    default:
        Q_ASSERT(false);
        qFatal("Unknown MappedLinkCmd Type");
        return false;
    }
}


///////////////////////////

GGDecisionLinkCmd::GGDecisionLinkCmd(GGEditModel *model, GGDecisionPage *page, const GGLink &lnk, Type type, int idx)
    : GGAbstractModelCommand(model),
      m_page(page),
      m_new(lnk),
      m_type(type),
      m_idx(idx)
{

}

GGDecisionLinkCmd::~GGDecisionLinkCmd()
{
    if (m_type == Remove) {
        if (m_state == Executed) {
            delete m_old.connection();
        }
    }
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
        Q_ASSERT_X(!m_new.connection() || m_new.connection() == m_old.connection(), "GGDecisionLinkCmd::doExecute", "New link has a connection");
        if (m_new.connection() && m_new.connection() != m_old.connection()) {
            return setError("New link has a connection");
        }
        // Same as Redo
        return doRedo();
    } else {
        Q_ASSERT_X(!m_new.connection(), "GGDecisionLinkCmd::doExecute", "New link has a connection");
        if (m_new.connection()) {
            return setError("New link has a connection");
        }
        m_idx = m_page->getDecisionLinks().size();
        m_page->addDecisionLink(m_new);
        return true;
    }
}

bool GGDecisionLinkCmd::doUndo()
{
    switch (m_type) {
    case Add:
        if (!m_page->removeDecisionLink(m_idx)) {
            return setError("Cannot remove decision link");
        }
        /*
         * New never has connection
        if (m_new.connection()) {
            bool ret = m_model->unregisterConnection(m_new.connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        */
        Q_ASSERT(!m_new.connection());
        return true;

    case Remove:
        if (!m_page->insertDecisionLink(m_idx, m_old)) {
            return setError("Cannot insert decision link");
        }
        if (m_old.connection()) {
            bool ret = m_model->registerConnectionWithId(m_old.connection());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;

    case Set:
        // Connection is unaffected. Simply copy back to old
        m_old.setConnection(m_new.connection());
        if (!m_page->setDecisionLink(m_idx, m_old)) {
            m_old.setConnection(NULL);
            return setError("Cannot set decision link");
        }
        m_new.setConnection(NULL);
        return true;

    default:
        Q_ASSERT(false);
        qFatal("Unknown DecisionLinkCmd Type");
        return false;
    }
}

bool GGDecisionLinkCmd::doRedo()
{
    switch (m_type) {
    case Add:
        /*
         * New never has connection
        if (m_new.connection()) {
            if (!m_model->registerConnectionWithId(m_new.connection())) {
                Q_ASSERT(false);
                return setError("Cannot register connection");
            }
        }
        */
        Q_ASSERT(!m_new.connection());
        m_page->addDecisionLink(m_new);
        return true;

    case Remove:
        if (!m_page->removeDecisionLink(m_idx)) {
            return setError("Cannot remove decision link");
        }
        if (m_old.connection()) {
            bool ret = m_model->unregisterConnection(m_old.connection()->id());
            Q_ASSERT(ret);
            Q_UNUSED(ret);
        }
        return true;

    case Set:
        // Connection is unaffected. Simply copy again to new
        m_new.setConnection(m_old.connection());
        if (!m_page->setDecisionLink(m_idx, m_new)) {
            m_new.setConnection(NULL);
            return setError("Cannot set decision link");
        }
        m_old.setConnection(NULL);
        return true;

    default:
        Q_ASSERT(false);
        qFatal("Unknown DecisionLinkCmd Type");
        return false;
    }
}
