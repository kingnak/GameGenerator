#include "ggmodelcommands.h"
#include "../model/ggeditmodel.h"
#include <model/ggabstractfactory.h>
#include <model/ggpage.h>

GGCreatePageCmd::GGCreatePageCmd(GGEditModel *model, PageType type)
    : GGAbstractModelCommand(model), m_type(type), m_createdPage(NULL)
{

}

GGCreatePageCmd::~GGCreatePageCmd()
{
    if (m_createdPage && m_createdPage->model() == NULL)
    {
        delete m_createdPage;
    }
}

bool GGCreatePageCmd::execute()
{
    switch (m_type) {
    case StartPage:
        m_createdPage = m_model->factory()->createStartPage();
        break;
    case EndPage:
        m_createdPage = m_model->factory()->createEndPage();
        break;
    case ConditionPage:
    case ActionPage:
    case DecisionPage:
        return setError("Page Type not yet supported");
    default:
        return setError("Unknown Page Type");
    }

    if (!m_model->registerNewPage(m_createdPage)) {
        return setError("Cannot register page");
    }

    return true;
}

bool GGCreatePageCmd::undo()
{
    // New page cannot have connections
    if (!m_model->unregisterPage(m_createdPage->id())) {
        return setError("Cannot unregister page");
    }
    return true;
}

bool GGCreatePageCmd::redo()
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

