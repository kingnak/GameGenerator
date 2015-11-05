#include "ggabstractcommand.h"

bool GGAbstractCommand::execute()
{
    if (m_state != NotExecuted) {
		//Q_ASSERT(false);
        return false;
    }

    if (doExecute()) {
        m_state = Executed;
        return true;
    }
    return false;
}

bool GGAbstractCommand::undo()
{
    if (m_state != Executed) {
		//Q_ASSERT(false);
        return false;
    }

    if (doUndo()) {
        m_state = Undone;
        return true;
    }
    return false;
}

bool GGAbstractCommand::redo()
{
    if (m_state != Undone) {
		//Q_ASSERT(false);
        return false;
    }

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
