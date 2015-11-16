#include "ggviewcommands.h"
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <command/ggabstractmodelcommandfactory.h>
#include <model/ggpage.h>

GGCreateViewPageCmd::GGCreateViewPageCmd(GGViewModel *model, GGCreatePageCmd::PageType type, QRect bounds)
    : GGAbstractViewForwardCommand(model),
      m_bounds(bounds)
{
    m_cmd = new GGCreatePageCmd(model->editModel(), type);
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

GGSetViewPageStringCmd::GGSetViewPageStringCmd(GGViewModel *model, GGViewPage *page, QString str, GGSetPageStringCmd::Type type)
    : GGAbstractViewForwardCommand(model)
{
    m_cmd = new GGSetPageStringCmd(model->editModel(), page->page(), str, type);
}

/////////////////////////

GGExchangeViewContentCmd::GGExchangeViewContentCmd(GGViewModel *model, GGViewPage *page, GGContentElement *elem)
    : GGAbstractViewForwardCommand(model)
{
    GGContentPage *p = GG::as<GGContentPage> (page->page());
    Q_ASSERT_X(p, "GGExchangeViewContentCmd::GGExchangeViewContentCmd", "Page must be content page");
    if (p) m_cmd = new GGExchangeContentCmd(model->editModel(), p, elem);
}

/////////////////////////

GGSetViewActionLinkCmd::GGSetViewActionLinkCmd(GGViewModel *model, GGViewPage *page, GGLink lnk)
    : GGAbstractViewForwardCommand(model)
{
    GGActionPage *p = ggpage_cast<GGActionPage *> (page->page());
    Q_ASSERT_X(p, "GGSetViewActionLinkCmd::GGSetViewActionLinkCmd", "Page must be action page");
    if (p) m_cmd = new GGSetActionLinkCmd(model->editModel(), p, lnk);
}

/////////////////////////

GGViewMappedLinkCmd::GGViewMappedLinkCmd(GGViewModel *model, GGViewPage *page, GGMappedLink lnk, GGMappedLinkCmd::Type type, int idx)
    : GGAbstractViewForwardCommand(model)
{
    GGMappedContentPage *p = GG::as<GGMappedContentPage> (page->page());
    Q_ASSERT_X(p, "GGViewMappedLinkCmd::GGViewMappedLinkCmd", "Page must be mappedcontent page");
    if (p) m_cmd = new GGMappedLinkCmd(model->editModel(), p, lnk, type, idx);
}

/////////////////////////

GGViewDecisionLinkCmd::GGViewDecisionLinkCmd(GGViewModel *model, GGViewPage *page, GGLink lnk, GGDecisionLinkCmd::Type type, int idx)
    : GGAbstractViewForwardCommand(model)
{
    GGDecisionPage *p = GG::as<GGDecisionPage> (page->page());
    Q_ASSERT_X(p, "GGViewDecisionLinkCmd::GGViewDecisionLinkCmd", "Page must be decision page");
    if (p) m_cmd = new GGDecisionLinkCmd(model->editModel(), p, lnk, type, idx);
}
