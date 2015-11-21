#include "gguicontroller.h"
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <command/ggcommandstack.h>
#include <viewcommand/ggviewcommands.h>
#include <viewmodel/ggviewmodel.h>
#include <viewcommand/ggviewcommandfactory.h>

GGUIController::GGUIController(QObject *parent)
    : QObject(parent)
{
    GGEditModel *em = new GGEditModel(new GGSimpleFactory, this);
    m_model = new GGViewModel(em, this);
    m_cmdFactory = new GGViewCommandFactory(m_model);
    m_stack = new GGCommandStack;
}

GGUIController::~GGUIController()
{
    delete m_cmdFactory;
    delete m_stack;
}

void GGUIController::undo()
{
    if (GGAbstractCommand *c = m_stack->undoCommand()) {
        if (!m_stack->undo()) {
            emit commandError(c->error());
        }
    }
}

void GGUIController::redo()
{
    if (GGAbstractCommand *c = m_stack->redoCommand()) {
        if (!m_stack->redo()) {
            emit commandError(c->error());
        }
    }
}

void GGUIController::changePageGeometry(GGViewPage *page, QRect rect)
{
    GGAbstractCommand *c = m_cmdFactory->movePage(page, rect);
    if (!m_stack->execute(c)) {
        emit commandError(c->error());
    }
}
