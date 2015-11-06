#ifndef GGCOMMANDSTACK_H
#define GGCOMMANDSTACK_H

#include <QList>
#include <QString>

class GGAbstractCommand;

class GGCommandStack
{
public:
    GGCommandStack();
    ~GGCommandStack();

    bool execute(GGAbstractCommand *cmd, bool deleteIfFailed = true);
    bool undo();
    bool redo();

    QString lastError();

    GGAbstractCommand *lastCommand();
    GGAbstractCommand *undoCommand();
    GGAbstractCommand *redoCommand();

    QList<GGAbstractCommand *> getAllCommands();
    QList<GGAbstractCommand *> getUndoCommands();
    QList<GGAbstractCommand *> getRedoCommands();

private:
    void purge();

private:
    QList<GGAbstractCommand *> m_stack;
    int m_commandIndex;
    QString m_lastError;
    GGAbstractCommand *m_lastCommand;
};

#endif // GGCOMMANDSTACK_H
