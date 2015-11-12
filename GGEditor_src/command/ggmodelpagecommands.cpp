#include "ggmodelpagecommands.h"
#include "../model/ggeditmodel.h"
#include <model/ggabstractfactory.h>
#include <model/ggpage.h>
#include <model/ggconnection.h>

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
