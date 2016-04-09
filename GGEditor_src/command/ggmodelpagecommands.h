#ifndef GGMODELPAGECOMMANDS_H
#define GGMODELPAGECOMMANDS_H

#include "ggabstractmodelcommand.h"
#include <model/ggconnectionslot.h>
#include <model/ggmappedlink.h>
#include <model/ggcondition.h>
#include <QList>

class GGScene;
class GGPage;
class GGConditionPage;
class GGContentPage;
class GGActionPage;
class GGMappedContentPage;
class GGDecisionPage;
class GGConnection;
class GGContentElement;

class GGCreateSceneCmd : public GGAbstractModelCommand
{
public:
    GGCreateSceneCmd(GGEditModel *model, const QString &name);
    ~GGCreateSceneCmd();

    QString description() const;

    GGScene *createdScene();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGScene *m_createdScene;
    QString m_name;
};

///////////////////////////

class GGRenameSceneCmd : public GGAbstractModelCommand
{
public:
    GGRenameSceneCmd(GGEditModel *model, GGScene *scene, const QString &newName);
    ~GGRenameSceneCmd();

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGScene *m_scene;
    QString m_newName;
    QString m_oldName;
};

///////////////////////////

class GGDeleteSceneCmd : public GGAbstractModelCommand
{
public:
    GGDeleteSceneCmd(GGEditModel *model, GGScene *scene);
    ~GGDeleteSceneCmd();

    QString description() const;

    GGScene *deletedScene();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGScene *m_deletedScene;
};

///////////////////////////

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

    GGCreatePageCmd(GGEditModel *model, GGScene *scene, PageType type);
    ~GGCreatePageCmd();

    QString description() const;

    GGPage *createdPage();

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGScene *m_scene;
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

class GGSetPageStringCmd : public GGAbstractModelCommand
{
public:
    enum Type {
        Name,
        //SceneName,
        Caption
    };
    GGSetPageStringCmd(GGEditModel *model, GGPage *page, const QString &str, Type type);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    bool doSet(QString v, QString *old = NULL);
    QString cmdDesc() const;

private:
    GGPage *m_page;
    QString m_new;
    QString m_old;
    Type m_type;
};

///////////////////////////

class GGExchangeConditionCmd : public GGAbstractModelCommand
{
public:
    GGExchangeConditionCmd(GGEditModel *model, GGConditionPage *page, const GGCondition &newCond);
    ~GGExchangeConditionCmd();

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGConditionPage *m_page;
    GGCondition m_new;
    GGCondition m_old;
};

///////////////////////////

class GGExchangeContentCmd : public GGAbstractModelCommand
{
public:
    GGExchangeContentCmd(GGEditModel *model, GGContentPage *page, GGContentElement *newElement);
    ~GGExchangeContentCmd();

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGContentPage *m_page;
    GGContentElement *m_new;
    GGContentElement *m_old;
};

///////////////////////////

class GGSetActionLinkCmd : public GGAbstractModelCommand
{
public:
    GGSetActionLinkCmd(GGEditModel *model, GGActionPage *page, const GGLink &lnk);

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    GGActionPage *m_page;
    GGLink m_new;
    GGLink m_old;
};

///////////////////////////

class GGMappedLinkCmd : public GGAbstractModelCommand
{
public:
    enum Type {
        Add, Remove, Set
    };
    GGMappedLinkCmd(GGEditModel *model, GGMappedContentPage *page, const GGMappedLink &lnk, Type type, int idx);
    ~GGMappedLinkCmd();

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    bool doSet(GGMappedLink n, GGMappedLink o);

private:
    GGMappedContentPage *m_page;
    GGMappedLink m_new;
    GGMappedLink m_old;
    Type m_type;
    int m_idx;
};

///////////////////////////

class GGDecisionLinkCmd : public GGAbstractModelCommand
{
public:
    enum Type {
        Add, Remove, Set
    };
    GGDecisionLinkCmd(GGEditModel *model, GGDecisionPage *page, const GGLink &lnk, Type type, int idx);
    ~GGDecisionLinkCmd();

    QString description() const;

protected:
    bool doExecute();
    bool doUndo();
    bool doRedo();

private:
    bool doSet(GGLink n, GGLink o);

private:
    GGDecisionPage *m_page;
    GGLink m_new;
    GGLink m_old;
    Type m_type;
    int m_idx;
};

#endif // GGMODELPAGECOMMANDS_H
