#include "ggviewcommands.h"
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <model/ggeditmodel.h>
#include <command/ggabstractmodelcommandfactory.h>

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
    if (!m_model->registerPage(new GGViewPage(m_cmd->createdPage(), m_model, m_bounds))) {
        bool ret = m_cmd->undo();
        Q_ASSERT(ret);
        Q_UNUSED(ret);
        return setError("Cannot register page");
    }
    return true;
}
