#ifndef GGCOMMANDGROUP_H
#define GGCOMMANDGROUP_H

#include "ggabstractcommand.h"
#include <QList>

class GGCommandStack;

class GGCommandGroup : public GGAbstractCommand
{
public:
    GGCommandGroup();
    ~GGCommandGroup();

    virtual QString description() const;

    void addCommand(GGAbstractCommand *cmd);
    QList<GGAbstractCommand *> commands();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    QList<GGAbstractCommand *> m_commands;

    friend class GGCommandStack;
};

#endif // GGCOMMANDGROUP_H
