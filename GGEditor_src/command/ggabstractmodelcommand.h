#ifndef GGABSTRACTMODELCOMMAND_H
#define GGABSTRACTMODELCOMMAND_H

#include <QString>

class GGEditModel;

class GGAbstractModelCommand
{
public:
    GGAbstractModelCommand(GGEditModel *model)
        : m_model(model), m_state(NotExecuted) {}
    virtual ~GGAbstractModelCommand() {}

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
    GGEditModel *m_model;
    CommandState m_state;
    QString m_error;
};

#endif // GGABSTRACTMODELCOMMAND_H
