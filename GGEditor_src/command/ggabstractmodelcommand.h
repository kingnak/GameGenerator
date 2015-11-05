#ifndef GGABSTRACTMODELCOMMAND_H
#define GGABSTRACTMODELCOMMAND_H

#include <QString>

class GGEditModel;

class GGAbstractModelCommand
{
public:
    GGAbstractModelCommand(GGEditModel *model)
        : m_model(model) {}
    virtual ~GGAbstractModelCommand() {}

    /*
    enum CommandState {

    };
    */

    virtual bool execute() = 0;
    virtual bool undo() = 0;
    virtual bool redo() = 0;
    virtual QString description() const = 0;
    virtual QString error() const { return m_error; }


protected:
    bool setError(QString error) {
        m_error = error;
        return false;
    }

protected:
    GGEditModel *m_model;
    QString m_error;
};

#endif // GGABSTRACTMODELCOMMAND_H
