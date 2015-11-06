#include "ggcommandgroup.h"

GGCommandGroup::GGCommandGroup()
{
}

GGCommandGroup::~GGCommandGroup()
{
    qDeleteAll(m_commands);
}

QString GGCommandGroup::description() const
{
    return "Multiple Commands";
}

void GGCommandGroup::addCommand(GGAbstractCommand *cmd)
{
    Q_ASSERT(m_state == NotExecuted);
    Q_ASSERT(cmd->state() == NotExecuted);
    if (m_state != NotExecuted || cmd->state() != NotExecuted) {
        return;
    }

    m_commands << cmd;
}

QList<GGAbstractCommand *> GGCommandGroup::commands()
{
    return m_commands;
}

bool GGCommandGroup::doExecute()
{
    for (int i = 0; i < m_commands.size(); ++i) {
        bool ret;
        // Command might be undone by previous failed execute
        if (m_commands[i]->state() == NotExecuted)
            ret = m_commands[i]->execute();
        else if (m_commands[i]->state() == Undone)
            ret = m_commands[i]->redo();

        if (!ret) {
            setError(m_commands[i]->error());
            for (int j = i-1; j >= 0; --j) {
                if (!m_commands[j]->undo()) {
                    Q_ASSERT_X(false, "GGCommandGroup::doExecute", qPrintable("Error while rolling back in failed execute: " + m_commands[j]->error()));
                    break;
                }
            }
            return false;
        }
    }

    return true;
}

bool GGCommandGroup::doUndo()
{
    for (int i = m_commands.size()-1; i >= 0; --i) {
        if (!m_commands[i]->undo()) {
            setError(m_commands[i]->error());
            for (int j = i+1; j < m_commands.size(); ++j) {
                if (!m_commands[j]->redo()) {
                    Q_ASSERT_X(false, "GGCommandGroup::doUndo", qPrintable("Error while rolling back in failed undo: " + m_commands[j]->error()));
                    break;
                }
            }
            return false;
        }
    }

    return true;
}

bool GGCommandGroup::doRedo()
{
    for (int i = 0; i < m_commands.size(); ++i) {
        if (!m_commands[i]->redo()) {
            setError(m_commands[i]->error());
            for (int j = i-1; j >= 0; --j) {
                if (!m_commands[j]->undo()) {
                    Q_ASSERT_X(false, "GGCommandGroup::doRedo", qPrintable("Error while rolling back in failed redo: " + m_commands[j]->error()));
                    break;
                }
            }
            return false;
        }
    }

    return true;
}
