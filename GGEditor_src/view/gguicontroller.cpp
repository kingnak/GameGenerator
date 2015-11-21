#include "gguicontroller.h"
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <command/ggcommandstack.h>
#include <command/ggcommandgroup.h>
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
    doExecCmd(m_cmdFactory->movePage(page, rect));

}

void GGUIController::changeMultiplePagesGeometry(QList<QPair<GGViewPage *, QRect> > changes)
{
    GGCommandGroup *grp = new GGCommandGroup;
    typedef QPair<GGViewPage*,QRect> PR;
    foreach (PR p, changes) {
        GGAbstractCommand *c = m_cmdFactory->movePage(p.first, p.second);
        grp->addCommand(c);
    }

    doExecCmd(grp);
}

void GGUIController::doExecCmd(GGAbstractCommand *cmd)
{
    if (!m_stack->execute(cmd)) {
        emit commandError(cmd->error());
    }
}
