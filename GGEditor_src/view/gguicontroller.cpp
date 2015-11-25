#include "gguicontroller.h"
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <command/ggcommandstack.h>
#include <command/ggcommandgroup.h>
#include <viewcommand/ggviewcommands.h>
#include <viewmodel/ggviewmodel.h>
#include <viewcommand/ggviewcommandfactory.h>

GGUIController::GGUIController(QObject *parent)
    : QObject(parent),
      m_model(NULL),
      m_cmdFactory(NULL),
      m_createMode(CreateNone),
      m_saveCommand(NULL)
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
    saveCheckpoint();
}

void GGUIController::saveCheckpoint()
{
    m_saveCommand = m_stack->undoCommand();
    checkSaveCheckpoint();
}

void GGUIController::undo()
{
    if (GGAbstractCommand *c = m_stack->undoCommand()) {
        if (!m_stack->undo()) {
            emit commandError(c->error());
        }
    }
    checkSaveCheckpoint();
}

void GGUIController::redo()
{
    if (GGAbstractCommand *c = m_stack->redoCommand()) {
        if (!m_stack->redo()) {
            emit commandError(c->error());
        }
    }
    checkSaveCheckpoint();
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

void GGUIController::deleteMultipleObjects(QSet<GGViewPage *> pages, QSet<GGViewConnection *> connections)
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

void GGUIController::setSelection(QSet<GGViewPage *> pages, QSet<GGViewConnection *> connections)
{
    if (pages.empty() && connections.empty()) {
        emit selectionCleared();
        return;
    }
    // TODO: don't emit when single conn/page selected?
    emit objectsSelected(pages, connections);
    if (pages.empty() && connections.size() == 1) {
        emit singleConnectionSelected(*connections.begin());
        return;
    }
    if (connections.empty() && pages.size() == 1) {
        emit singlePageSelected(*pages.begin());
        return;
    }
}

void GGUIController::handleSceneClick(QPointF pos)
{
    GGCreateViewPageCmd *cmd = NULL;
    QRect r(pos.toPoint(), QSize());
    switch (m_createMode) {
    case CreateNone:
        return;
    case CreateStartPage:
        cmd = m_cmdFactory->createStartPage(r);
        break;
    case CreateEndPage:
        cmd = m_cmdFactory->createEndPage(r);
        break;
    case CreateConditionPage:
        cmd = m_cmdFactory->createConditionPage(r);
        break;
    case CreateActionPage:
        cmd = m_cmdFactory->createActionPage(r);
        break;
    case CreateDecisionPage:
        cmd = m_cmdFactory->createDecisionPage(r);
        break;
    case CreateConnection:
        return;
    }
    if (cmd) {
        if (doExecCmd(cmd)) {
            // This doesn't work, as the scene somehow immediately resets the selection...
            this->setSelection(QSet<GGViewPage*>()<<cmd->createdPage(), QSet<GGViewConnection*>());
        }
    }
}

bool GGUIController::doExecCmd(GGAbstractCommand *cmd)
{
    if (!m_stack->execute(cmd)) {
        emit commandError(cmd->error());
        return false;
    }
    checkSaveCheckpoint();
    return true;
}

bool GGUIController::checkSaveCheckpoint()
{
    bool dirty = (m_saveCommand != m_stack->undoCommand());
    emit modelDirty(dirty);

    bool canUndo = (m_stack->undoCommand() != NULL);
    bool canRedo = (m_stack->redoCommand() != NULL);
    emit undoAvailable(canUndo);
    emit redoAvailable(canRedo);

    return !dirty;
}
