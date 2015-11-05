#ifndef GGMODELCOMMANDS_H
#define GGMODELCOMMANDS_H

#include "ggabstractmodelcommand.h"

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

#endif // GGMODELCOMMANDS_H
