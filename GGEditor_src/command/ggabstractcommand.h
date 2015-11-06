#ifndef GGABSTRACTCOMMAND_H
#define GGABSTRACTCOMMAND_H

#include <QString>

class GGAbstractCommand
{
public:
    GGAbstractCommand()
        : m_state(NotExecuted) {}
    virtual ~GGAbstractCommand() {}

    enum CommandState {
        NotExecuted,
        Executed,
        Undone
    };

    CommandState state() const { return m_state; }

    bool execute();
    bool undo();
    bool redo();

    QString error() const { return m_error; }
    virtual QString description() const = 0;


protected:
    bool setError(QString error);
    virtual bool doExecute() = 0;
    virtual bool doUndo() = 0;
    virtual bool doRedo() = 0;

protected:
    CommandState m_state;
    QString m_error;
};

#endif // GGABSTRACTCOMMAND_H
