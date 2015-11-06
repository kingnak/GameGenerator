#ifndef GGMODELPAGECOMMANDS_H
#define GGMODELPAGECOMMANDS_H

#include "ggabstractmodelcommand.h"
#include <model/ggconnectionslot.h>
#include <QList>

class GGPage;
class GGConnection;

class GGCreatePageCmd : public GGAbstractModelCommand
{
public:
    enum PageType {
        StartPage,
        EndPage,
        ConditionPage,
        ActionPage,
        DecisionPage
    };

    GGCreatePageCmd(GGEditModel *model, PageType type);
    ~GGCreatePageCmd();

    QString description() const;

    GGPage *createdPage();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    PageType m_type;
    GGPage *m_createdPage;
};

///////////////////////////

class GGDeletePageCmd : public GGAbstractModelCommand
{
public:
    GGDeletePageCmd(GGEditModel *model, GGPage *page);
    ~GGDeletePageCmd();

    QString description() const;

    GGPage *deletedPage();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGPage *m_deletedPage;
    struct PCS {
        GGConnectionSlot slot;
        GGPage *page;
        GGConnection *conn;
    };

    QList<PCS> m_slots;
    QList<GGConnection *> m_affectedConnections;
};

///////////////////////////


#endif // GGMODELPAGECOMMANDS_H
