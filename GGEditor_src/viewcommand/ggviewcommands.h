#ifndef GGVIEWCOMMANDS_H
#define GGVIEWCOMMANDS_H

#include <command/ggabstractcommand.h>
#include <command/ggmodelpagecommands.h>

class GGViewModel;

class GGAbstractViewCommand : public GGAbstractCommand
{
public:
    GGAbstractViewCommand(GGViewModel *model) : m_model(model) {}

protected:
    GGViewModel *m_model;
};

/////////////////////////

template<class CMD>
class GGAbstractViewForwardCommand : public GGAbstractViewCommand
{
public:
    GGAbstractViewForwardCommand(GGViewModel *model) : GGAbstractViewCommand(model) {}
    ~GGAbstractViewForwardCommand() { delete m_cmd; }

    CMD *getInnerCommand() { return m_cmd; }

public:
    QString description() const { return m_cmd->description(); }

protected:
    bool doUndo();
    bool doRedo();

protected:
    CMD *m_cmd;
};

/////////////////////////

class GGCreateViewPageCmd : public GGAbstractViewForwardCommand<GGCreatePageCmd>
{
public:
    GGCreateViewPageCmd(GGViewModel *model, GGCreatePageCmd::PageType type, QRect bounds);

protected:
    bool doExecute();

private:
    QRect m_bounds;
};

/////////////////////////

template<typename CMD>
bool GGAbstractViewForwardCommand<CMD>::doUndo() {
    if (!m_cmd->undo()) return setError(m_cmd->error());
    return true;
}

template<typename CMD>
bool GGAbstractViewForwardCommand<CMD>::doRedo() {
    if (!m_cmd->redo()) return setError(m_cmd->error());
    return true;
}

#endif // GGVIEWCOMMANDS_H
