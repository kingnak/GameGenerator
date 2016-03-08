#include "gguicontroller.h"
#include <model/ggeditmodel.h>
#include <model/ggsimplefactory.h>
#include <model/ggpage.h>
#include <command/ggcommandstack.h>
#include <command/ggcommandgroup.h>
#include <viewcommand/ggviewcommands.h>
#include <viewmodel/ggviewmodel.h>
#include <viewmodel/ggviewpage.h>
#include <viewcommand/ggviewcommandfactory.h>
#include <ui/dialogs/ggchoseconnectionslotdlg.h>

GGUIController::GGUIController(QObject *parent)
    : QObject(parent),
      m_model(NULL),
      m_cmdFactory(NULL),
      m_createMode(CreateNone),
      m_saveCommand(NULL),
      m_directConnSource(NULL),
      m_directConnSlot(GGConnectionSlot::NoConnection)
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

void GGUIController::setCreationMode(GGUIController::CreationMode mode)
{
    m_directConnSlot = GGConnectionSlot::NoConnection;
    m_directConnSource = NULL;
    if (mode != m_createMode) {
        m_createMode = mode;
        emit creationModeChanged(mode);
    }
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

void GGUIController::deleteConnection(GGConnection *c)
{
    doExecCmd(m_cmdFactory->deleteConnection(c));
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

void GGUIController::changePageName(GGPage *page, QString name)
{
    doExecCmd(m_cmdFactory->setPageName(page, name));
}

void GGUIController::changePageScene(GGPage *page, QString scene)
{
    doExecCmd(m_cmdFactory->setPageScene(page, scene));
}

void GGUIController::changeContentPageCaption(GGContentPage *page, QString cap)
{
    doExecCmd(m_cmdFactory->setPageCaption(page, cap));
}

void GGUIController::changeContentElement(GGContentPage *page, GGContentElement *elem)
{
    doExecCmd(m_cmdFactory->exchangeContent(page, elem));
}

void GGUIController::changeLink(GGPage *page, GGConnectionSlot slot, const GGLink &link)
{
    GGAbstractCommand *cmd = NULL;
    if (slot.type() == GGConnectionSlot::ActionConnection) {
        cmd = m_cmdFactory->setActionLink(GG::as<GGActionPage>(page), link);
    } else if (slot.type() == GGConnectionSlot::DecisionConnection) {
        cmd = m_cmdFactory->setDecisionLink(GG::as<GGDecisionPage>(page), slot.index(), link);
    } else if (slot.type() == GGConnectionSlot::MappedConnection) {
        GGMappedContentPage *mcp = GG::as<GGMappedContentPage>(page);
        GGMappedLink ml = mcp->getLinkMap().value(slot.index());
        ml.setLink(link);
        cmd = m_cmdFactory->setMappedLink(mcp, slot.index(), ml);
    } else {
        Q_ASSERT_X(false, "GGUIController::changeLink", "Unsupported connection type");
        return;
    }
    doExecCmd(cmd);
}

void GGUIController::removeDecisionLink(GGDecisionPage *page, int idx)
{
    doExecCmd(m_cmdFactory->removeDecisionLink(page, idx));
}

void GGUIController::addDecisionLink(GGDecisionPage *page)
{
    doExecCmd(m_cmdFactory->addDecisionLink(page));
}

void GGUIController::connnectPagesDialog(GGViewPage *src, GGViewPage *dest)
{
    setCreationMode(CreateNone);
    // DUMMY
    /*
    QList<GGConnectionSlot> lst = GGConnectionSlot::enumerateConnections(src->page());
    if (!lst.isEmpty()) {
        GGCreateViewConnectionCmd *cmd = m_cmdFactory->createConnection(src, dest, lst[0]);
        if (doExecCmd(cmd))
            if (cmd->createdConnection())
                setSelection(QSet<GGViewPage*>(), QSet<GGViewConnection *>() << cmd->createdConnection());
    }
    */
    GGChoseConnectionSlotDlg dlg;
    dlg.setConnectionSlots(src->page());
    if (dlg.exec() == QDialog::Accepted) {
        GGConnectionSlot s = dlg.selectedSlot();
        GGCreateViewConnectionCmd *cmd = m_cmdFactory->createConnection(src, dest, s);
        if (doExecCmd(cmd))
            if (cmd->createdConnection())
                setSelection(QSet<GGViewPage*>(), QSet<GGViewConnection *>() << cmd->createdConnection());
    }
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
    case CreateConnectionDirect:
        return;
    }
    if (cmd) {
        if (doExecCmd(cmd)) {
            // This doesn't work, as the scene somehow immediately resets the selection...
            this->setSelection(QSet<GGViewPage*>()<<cmd->createdPage(), QSet<GGViewConnection*>());
        }
    }
}

void GGUIController::connectPageDirect(GGPage *src, GGConnectionSlot slot)
{
    setCreationMode(CreateConnectionDirect);
    m_directConnSource = src;
    m_directConnSlot = slot;
    emit connectingDirect(src, slot);
}

void GGUIController::setDirectConnectionPage(GGPage *dest)
{
    doExecCmd(m_cmdFactory->createConnection(m_model->getViewPageForPage(m_directConnSource), m_model->getViewPageForPage(dest), m_directConnSlot));
    abortDirectConnection();
}

void GGUIController::abortDirectConnection()
{
    setCreationMode(CreateNone);
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
