#ifndef GGCOMMANDSTACK_H
#define GGCOMMANDSTACK_H

#include <QList>
#include <QString>

class GGAbstractCommand;
class GGCommandGroup;

class GGCommandStack
{
public:
    GGCommandStack();
    ~GGCommandStack();

    bool execute(GGAbstractCommand *cmd, bool deleteIfFailed = true);
    bool undo();
    bool redo();

    void clear();

    QString lastError();

    GGAbstractCommand *lastCommand();
    GGAbstractCommand *undoCommand();
    GGAbstractCommand *redoCommand();

    QList<GGAbstractCommand *> getAllCommands() const;
    QList<GGAbstractCommand *> getUndoCommands() const;
    QList<GGAbstractCommand *> getRedoCommands() const;

    void merge(GGCommandStack &other);
    void mergeAsGroup(GGCommandStack &other);
    GGCommandGroup *toCommandGroup();

private:
    void purge();

private:
    QList<GGAbstractCommand *> m_stack;
    int m_commandIndex;
    QString m_lastError;
    GGAbstractCommand *m_lastCommand;
};

#endif // GGCOMMANDSTACK_H
