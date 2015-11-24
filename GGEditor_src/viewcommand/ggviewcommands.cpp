#include "ggviewcommands.h"
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <viewmodel/ggviewconnection.h>
#include <model/ggeditmodel.h>
#include <command/ggabstractmodelcommandfactory.h>
#include <model/ggpage.h>

GGCreateViewPageCmd::GGCreateViewPageCmd(GGViewModel *model, GGCreatePageCmd::PageType type, QRect bounds)
    : GGAbstractViewForwardCommand(model),
      m_bounds(bounds)
{
    m_cmd = new GGCreatePageCmd(model->editModel(), type);
}

GGViewPage *GGCreateViewPageCmd::createdPage()
{
    if (state() == Executed) {
        return m_model->getViewPageForPage(getInnerCommand()->createdPage());
    }
    return NULL;
}

bool GGCreateViewPageCmd::doExecute()
{
    if (!m_cmd->execute()) {
        return setError(m_cmd->error());
    }
    /* This is required if using QueuedConnections
    if (!m_model->registerPage(new GGViewPage(m_cmd->createdPage(), m_model, m_bounds))) {
        bool ret = m_cmd->undo();
        Q_ASSERT(ret);
        Q_UNUSED(ret);
        return setError("Cannot register page");
    }
    */
    GGViewPage *vp = m_model->getViewPageForPage(m_cmd->createdPage());
    Q_ASSERT(vp);
    vp->setBounds(m_bounds);
    return true;
}

/////////////////////////

GGDeleteViewPageCmd::GGDeleteViewPageCmd(GGViewModel *model, GGViewPage *page)
    : GGAbstractViewForwardCommand(model)
{
    m_cmd = new GGDeletePageCmd(model->editModel(), page->page());
}

/////////////////////////

GGMoveViewPageCmd::GGMoveViewPageCmd(GGViewModel *model, GGViewPage *page, QRect pos)
    : GGAbstractViewCommand(model),
      m_page(page),
      m_new(pos)
{
}

QString GGMoveViewPageCmd::description() const
{
    return "Move Page";
}

bool GGMoveViewPageCmd::doExecute()
{
    m_old = m_page->bounds();
    m_page->setBounds(m_new);
    return true;
}

bool GGMoveViewPageCmd::doUndo()
{
    m_page->setBounds(m_old);
    return true;
}

bool GGMoveViewPageCmd::doRedo()
{
    m_page->setBounds(m_new);
    return true;
}

/////////////////////////

GGCreateViewConnectionCmd::GGCreateViewConnectionCmd(GGViewModel *model, GGConnectionSlot slot, GGViewPage *src, GGViewPage *dest)
    : GGAbstractViewForwardCommand(model)
{
    m_cmd = new GGCreateConnectionCmd(model->editModel(), slot, src->page(), dest->page());
}

/////////////////////////

GGDeleteViewConnectionCmd::GGDeleteViewConnectionCmd(GGViewModel *model, GGViewConnection *conn)
    : GGAbstractViewForwardCommand(model)
{
    m_cmd = new GGDeleteConnectionCmd(model->editModel(), conn->connection());
}

/////////////////////////
