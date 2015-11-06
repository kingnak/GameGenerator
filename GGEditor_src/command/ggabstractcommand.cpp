#include "ggabstractcommand.h"

bool GGAbstractCommand::execute()
{
    Q_ASSERT(m_state == NotExecuted);
    if (m_state != NotExecuted) {
        return setError("Command is not in NotExecuted state");
    }

    m_error.clear();
    if (doExecute()) {
        m_state = Executed;
        return true;
    }
    return false;
}

bool GGAbstractCommand::undo()
{
    Q_ASSERT(m_state == Executed);
    if (m_state != Executed) {
        return setError("Command is not in Executed state");
    }

    m_error.clear();
    if (doUndo()) {
        m_state = Undone;
        return true;
    }
    return false;
}

bool GGAbstractCommand::redo()
{
    Q_ASSERT(m_state == Undone);
    if (m_state != Undone) {
        return setError("Command is not in Undone state");
    }

    m_error.clear();
    if (doRedo()) {
        m_state = Executed;
        return true;
    }
    return false;
}

bool GGAbstractCommand::setError(QString error)
{
    m_error = error;
    return false;
}
