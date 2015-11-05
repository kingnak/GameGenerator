#include "ggabstractmodelcommand.h"

bool GGAbstractModelCommand::execute()
{
    if (m_state != NotExecuted) {
        Q_ASSERT(false);
        return false;
    }

    if (doExecute()) {
        m_state = Executed;
        return true;
    }
    return false;
}

bool GGAbstractModelCommand::undo()
{
    if (m_state != Executed) {
        Q_ASSERT(false);
        return false;
    }

    if (doUndo()) {
        m_state = Undone;
        return true;
    }
    return false;
}

bool GGAbstractModelCommand::redo()
{
    if (m_state != Undone) {
        Q_ASSERT(false);
        return false;
    }

    if (doRedo()) {
        m_state = Executed;
        return true;
    }
    return false;
}

bool GGAbstractModelCommand::setError(QString error)
{
    m_error = error;
    return false;
}
