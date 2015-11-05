#ifndef GGCREATEPAGECMD_H
#define GGCREATEPAGECMD_H

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

    bool execute();
    bool undo();
    bool redo();
    QString description() const;

    GGPage *createdPage();

private:
    PageType m_type;
    GGPage *m_createdPage;
};

///////////////////////////

#endif // GGCREATEPAGECMD_H
