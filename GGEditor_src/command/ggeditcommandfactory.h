#ifndef GGEDITCOMMANDFACTORY_H
#define GGEDITCOMMANDFACTORY_H

#include "ggabstractmodelcommandfactory.h"

class GGEditModel;

class GGEditCommandFactory : public GGAbstractModelCommandFactory
{
public:
    GGEditCommandFactory(GGEditModel *model);

    GGCreateSceneCmd *createScene(const QString &name, const QString &mediaDir);
    GGDeleteSceneCmd *deleteScene(GGScene *scene);
    GGRenameSceneCmd *renameScene(GGScene *scene, const QString &newName);
    GGRenameSceneMediaDirCmd *renameSceneMediaDir(GGScene *scene, const QString &newName);

    GGCreatePageCmd *createStartPage(GGScene *scene);
    GGCreatePageCmd *createEndPage(GGScene *scene);
    GGCreatePageCmd *createConditionPage(GGScene *scene);
    GGCreatePageCmd *createActionPage(GGScene *scene);
    GGCreatePageCmd *createDecisionPage(GGScene *scene);

    GGDeletePageCmd *deletePage(GGPage *page);

    GGCreateConnectionCmd *createConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot);
    GGExchangeConnectionCmd *exchangeConnection(GGPage *src, GGPage *dest, const GGConnectionSlot &slot);
    GGDeleteConnectionCmd *deleteConnection(GGConnection *conn);

    GGSetPageStringCmd *setPageName(GGPage *p, const QString &name);
    GGSetPageStringCmd *setPageCaption(GGContentPage *p, const QString &caption);

    GGExchangeConditionCmd *exchangeCondition(GGConditionPage *p, const GGCondition &cond);
    GGExchangeContentCmd *exchangeContent(GGContentPage *p, GGContentElement *elem);

    GGSetActionLinkCmd *setActionLink(GGActionPage *p, const GGLink &lnk);

    GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p);
    GGMappedLinkCmd *addMappedLink(GGMappedContentPage *p, const GGMappedLink &lnk);
    GGMappedLinkCmd *removeMappedLink(GGMappedContentPage *p, int idx);
    GGMappedLinkCmd *setMappedLink(GGMappedContentPage *p, int idx, const GGMappedLink &lnk);

    GGDecisionLinkCmd *addDecisionLink(GGDecisionPage *p);
    GGDecisionLinkCmd *removeDecisionLink(GGDecisionPage *p, int idx);
    GGDecisionLinkCmd *setDecisionLink(GGDecisionPage *p, int idx, const GGLink &lnk);

    GGAddVariableCmd *addVariable(const QString &name);
    GGRemoveVariableCmd *removeVariable(const QString &name);
    GGChangeVariableCmd *updateVariable(const QString &name, const GGVariable &newData);

private:
    GGEditModel *m_model;
};

#endif // GGEDITCOMMANDFACTORY_H
