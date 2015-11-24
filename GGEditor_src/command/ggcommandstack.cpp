#include "ggcommandstack.h"
#include "ggabstractcommand.h"

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
    m_lastError.clear();;

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

QList<GGAbstractCommand *> GGCommandStack::getAllCommands()
{
    return m_stack;
}

QList<GGAbstractCommand *> GGCommandStack::getUndoCommands()
{
    // There is no reverse???
    QList<GGAbstractCommand *> ret;
    ret.reserve(m_commandIndex);
    for (int i = m_commandIndex; i >= 0; --i) {
        ret << m_stack[i];
    }
    return ret;
}

QList<GGAbstractCommand *> GGCommandStack::getRedoCommands()
{
    return m_stack.mid(m_commandIndex+1);
}

void GGCommandStack::purge()
{
    while (m_stack.size()-1 > m_commandIndex) {
        delete m_stack.takeLast();
    }
}

