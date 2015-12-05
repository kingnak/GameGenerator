#include "ggcommandstack.h"
#include "ggabstractcommand.h"
#include "ggcommandgroup.h"

GGCommandStack::GGCommandStack()
    : m_commandIndex(-1), m_lastCommand(NULL)
{
}

GGCommandStack::~GGCommandStack()
{
    qDeleteAll(m_stack);
}

bool GGCommandStack::execute(GGAbstractCommand *cmd, bool deleteIfFailed)
{
    purge();
    if (!cmd->execute()) {
        m_lastError = cmd->error();
        if (deleteIfFailed) {
            delete cmd;
        }
        return false;
    }
    m_lastError.clear();

    m_stack << cmd;
    m_commandIndex++;
    m_lastCommand = cmd;
    return true;
}

bool GGCommandStack::undo()
{
    GGAbstractCommand *c = undoCommand();
    m_lastError.clear();
    if (!c) {
        return false;
    }

    if (!c->undo()) {
        m_lastError = c->error();
        return false;
    }
    m_lastCommand = c;
    m_commandIndex--;
    return true;
}

bool GGCommandStack::redo()
{
    GGAbstractCommand *c = redoCommand();
    m_lastError.clear();
    if (!c) {
        return false;
    }

    if (!c->redo()) {
        m_lastError = c->error();
        return false;
    }
    m_commandIndex++;
    m_lastCommand = c;
    return true;
}

void GGCommandStack::clear()
{
    qDeleteAll(m_stack);
    m_stack.clear();
    m_lastCommand = NULL;
    m_lastError.clear();
    m_commandIndex = -1;
}

QString GGCommandStack::lastError()
{
    return m_lastError;
}

GGAbstractCommand *GGCommandStack::lastCommand()
{
    return m_lastCommand;
}

GGAbstractCommand *GGCommandStack::undoCommand()
{
    return m_commandIndex >= 0 ? m_stack[m_commandIndex] : NULL;
}

GGAbstractCommand *GGCommandStack::redoCommand()
{
    return (!m_stack.empty() && m_commandIndex < m_stack.size()-1) ? m_stack[m_commandIndex+1] : NULL;
}

QList<GGAbstractCommand *> GGCommandStack::getAllCommands() const
{
    return m_stack;
}

QList<GGAbstractCommand *> GGCommandStack::getUndoCommands() const
{
    // There is no reverse???
    QList<GGAbstractCommand *> ret;
    ret.reserve(m_commandIndex);
    for (int i = m_commandIndex; i >= 0; --i) {
        ret << m_stack[i];
    }
    return ret;
}

QList<GGAbstractCommand *> GGCommandStack::getRedoCommands() const
{
    return m_stack.mid(m_commandIndex+1);
}

void GGCommandStack::merge(GGCommandStack &other)
{
    if (other.getUndoCommands().isEmpty()) {
        // Don't merge empty stack
        return;
    }

    purge();
    m_stack << other.m_stack;
    m_lastCommand = other.m_lastCommand;
    m_lastError = other.m_lastError;
    m_commandIndex += other.m_commandIndex + 1;

    other.m_stack.clear();
    other.clear();
}

void GGCommandStack::mergeAsGroup(GGCommandStack &other)
{
    GGCommandGroup *grp = other.toCommandGroup();
    // Don't merge an empty group
    if (!grp || grp->commands().isEmpty()) {
        return;
    }

    purge();
    m_lastError.clear();

    m_stack << grp;
    m_commandIndex++;
    m_lastCommand = grp;
}

GGCommandGroup *GGCommandStack::toCommandGroup()
{
    purge();

    GGCommandGroup *ret = new GGCommandGroup;
    ret->m_commands << m_stack;
    ret->m_state = GGAbstractCommand::Executed;

    m_stack.clear();
    clear();

    if (ret->commands().isEmpty()) {
        delete ret;
        ret = NULL;
    }

    return ret;
}

void GGCommandStack::purge()
{
    while (m_stack.size()-1 > m_commandIndex) {
        delete m_stack.takeLast();
    }
}

