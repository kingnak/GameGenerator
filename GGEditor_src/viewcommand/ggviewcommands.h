#ifndef GGVIEWCOMMANDS_H
#define GGVIEWCOMMANDS_H

#include <command/ggabstractcommand.h>
#include <command/ggmodelpagecommands.h>
#include <command/ggmodelconnectioncommands.h>

class GGViewPage;
class GGViewConnection;
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
    GGAbstractViewForwardCommand(GGViewModel *model) : GGAbstractViewCommand(model), m_cmd(NULL) {}
    ~GGAbstractViewForwardCommand() { delete m_cmd; }

    CMD *getInnerCommand() { return m_cmd; }

public:
    QString description() const { return m_cmd->description(); }

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

protected:
    CMD *m_cmd;
};

/////////////////////////

template<typename CMD>
bool GGAbstractViewForwardCommand<CMD>::doExecute() {
    if (!m_cmd->execute()) return setError(m_cmd->error());
    return true;
}

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

/////////////////////////

class GGCreateViewPageCmd : public GGAbstractViewForwardCommand<GGCreatePageCmd>
{
public:
    GGCreateViewPageCmd(GGViewModel *model, GGScene *scene, GGCreatePageCmd::PageType type, const QRect &bounds);

    GGViewPage *createdPage();

protected:
    bool doExecute();

private:
    QRect m_bounds;
};

/////////////////////////

class GGDeleteViewPageCmd : public GGAbstractViewForwardCommand<GGDeletePageCmd>
{
public:
    GGDeleteViewPageCmd(GGViewModel *model, GGViewPage *page);
};

/////////////////////////

class GGMoveViewPageCmd : public GGAbstractViewCommand
{
public:
    GGMoveViewPageCmd(GGViewModel *model, GGViewPage *page, const QRect &pos);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGViewPage *m_page;
    QRect m_new;
    QRect m_old;
};

/////////////////////////

class GGCreateViewConnectionCmd : public GGAbstractViewForwardCommand<GGExchangeConnectionCmd>
{
public:
    GGCreateViewConnectionCmd(GGViewModel *model, const GGConnectionSlot &slot, GGViewPage *src, GGViewPage *dest);
    GGViewConnection *createdConnection();
protected:
    // Maybe in future needed
    //bool doExecute();
};

/////////////////////////

class GGDeleteViewConnectionCmd : public GGAbstractViewForwardCommand<GGDeleteConnectionCmd>
{
public:
    GGDeleteViewConnectionCmd(GGViewModel *model, GGViewConnection *conn);
};

#endif // GGVIEWCOMMANDS_H
