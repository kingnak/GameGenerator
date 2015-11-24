#include "gguicontroller.h"
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <command/ggcommandstack.h>
#include <command/ggcommandgroup.h>
#include <viewcommand/ggviewcommands.h>
#include <viewmodel/ggviewmodel.h>
#include <viewcommand/ggviewcommandfactory.h>

GGUIController::GGUIController(QObject *parent)
    : QObject(parent), m_model(NULL), m_cmdFactory(NULL)
{
    m_stack = new GGCommandStack;
}

GGUIController::~GGUIController()
{
    delete m_cmdFactory;
    delete m_stack;
}

void GGUIController::setModel(GGViewModel *model)
{
    delete m_cmdFactory;
    m_stack->clear();
    m_model = model;
    if (m_model)
        m_cmdFactory = new GGViewCommandFactory(m_model);
    else
        m_cmdFactory = NULL;
    emit modelReset(m_model);
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

void GGUIController::deleteMultipleObjects(QList<GGViewPage *> pages, QList<GGViewConnection *> connections)
{
    GGCommandGroup *grp = new GGCommandGroup;
    foreach (GGViewConnection *c, connections) {
        grp->addCommand(m_cmdFactory->deleteConnection(c));
    }
    foreach (GGViewPage *p, pages) {
        grp->addCommand(m_cmdFactory->deletePage(p));
    }
    doExecCmd(grp);
}

GGViewPage *GGUIController::createStartPage(QRect r)
{
    GGCreateViewPageCmd *c = m_cmdFactory->createStartPage(r);
    if (doExecCmd(c)) {
        return c->createdPage();
    }
    return NULL;
}

GGViewPage *GGUIController::createEndPage(QRect r)
{
    GGCreateViewPageCmd *c = m_cmdFactory->createEndPage(r);
    if (doExecCmd(c)) {
        return c->createdPage();
    }
    return NULL;
}

GGViewPage *GGUIController::createConditionPage(QRect r)
{
    GGCreateViewPageCmd *c = m_cmdFactory->createConditionPage(r);
    if (doExecCmd(c)) {
        return c->createdPage();
    }
    return NULL;
}

GGViewPage *GGUIController::createActionPage(QRect r)
{
    GGCreateViewPageCmd *c = m_cmdFactory->createActionPage(r);
    if (doExecCmd(c)) {
        return c->createdPage();
    }
    return NULL;
}

GGViewPage *GGUIController::createDecisionPage(QRect r)
{
    GGCreateViewPageCmd *c = m_cmdFactory->createDecisionPage(r);
    if (doExecCmd(c)) {
        return c->createdPage();
    }
    return NULL;
}

bool GGUIController::doExecCmd(GGAbstractCommand *cmd)
{
    if (!m_stack->execute(cmd)) {
        emit commandError(cmd->error());
        return false;
    }
    return true;
}
