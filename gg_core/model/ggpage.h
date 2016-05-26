#ifndef GGPAGE_H
#define GGPAGE_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>
#include "ggabstractmodel.h"
#include "ggmappedlink.h"
#include "gglink.h"
#include "ggcondition.h"
#include "ggsearch.h"

class GGConnection;
class GGAbstractModel;
class GGAbstractProjectUnserializer;
class GGContentElement;

class GG_CORESHARED_EXPORT GGPage
{
    Q_DISABLE_COPY(GGPage)

protected:
    GGPage(GG::SceneID scene);

public:
    virtual ~GGPage();

    virtual int type() const = 0;

    GG::PageID id() const { return m_id; }
    QString name() const { return m_name; }
    GGAbstractModel *model() const { return m_model; }

    GG::SceneID sceneId() const { return m_sceneId; }
    GGScene *scene() const { return m_scene; }
    bool setSceneId(GG::SceneID sceneId);
    void setName(QString n);

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

    virtual QList<GGConnection *> getConnections() const = 0;
    virtual bool removeConnection(GGConnection *connection) = 0;

private:
    GG::PageID m_id;
    GGAbstractModel *m_model;
    GG::SceneID m_sceneId;
    GGScene *m_scene;

protected:
    void notifyChanged(int sections);

protected:
    QString m_name;

    friend class GGAbstractModel;
    friend class GGAbstractProjectUnserializer;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGConditionPage : public GGPage
{
public:
    GGConditionPage(GG::SceneID scene);

    enum {
        Type = 0x0003
    };
    int type() const;

    GGConnection *trueConnection() const;
    GGConnection *falseConnection() const;

    void setTrueConnection(GGConnection *t);
    void setFalseConnection(GGConnection *f);

    GGCondition getCondition() const;
    void setCondition(const GGCondition &cond);

    QList<GGConnection *> getConnections() const;
    bool removeConnection(GGConnection *connection);

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

private:
    GGConnection *m_true;
    GGConnection *m_false;

    GGCondition m_cond;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGContentPage : public GGPage
{
public:
    GGContentPage(GG::SceneID scene);
    ~GGContentPage();

    QString caption() const { return m_caption; }
    void setCaption(QString cap);

    GGContentElement *content();
    const GGContentElement *content() const;
    void setContent(GGContentElement *cont);
    GGContentElement *exchangeContent(GGContentElement *cont);

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

protected:
    QString m_caption;
    GGContentElement *m_content;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGStartPage : public GGContentPage
{
public:
    GGStartPage(GG::SceneID scene);

    enum {
        Type = 0x0001
    };
    int type() const;

    void setStartConnection(GGConnection *conn);
    GGConnection *startConnection() const;

    QList<GGConnection *> getConnections() const;
    bool removeConnection(GGConnection *connection);

private:
    GGConnection *m_conn;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGEntryActionPage : public GGContentPage
{
public:
    GGEntryActionPage(GG::SceneID scene);
    ~GGEntryActionPage();

    GGAction entryAction() const { return m_entryAction; }
    void setEntryAction(GGAction action);

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

protected:
    GGAction m_entryAction;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGEndPage : public GGEntryActionPage
{
public:
    GGEndPage(GG::SceneID scene);

    enum {
        Type = 0x0002
    };
    int type() const;

    QList<GGConnection *> getConnections() const;
    bool removeConnection(GGConnection *connection);
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGMappedContentPage : public GGEntryActionPage
{
public:
    GGMappedContentPage(GG::SceneID scene);

    bool removeConnection(GGConnection *connection);

    QList<GGConnection *> getMappedConnections() const;
    void addMappedLink(GGMappedLink link);
    bool insertMappedLink(int idx, GGMappedLink link);
    bool setMappedLink(int idx, GGMappedLink link);
    bool removeMappedLink(int idx);
    QList<GGMappedLink> getLinkMap() const;

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

protected:
    QList<GGMappedLink> m_mappedLinks;
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGActionPage : public GGMappedContentPage
{
public:
    GGActionPage(GG::SceneID scene);

    enum {
        Type = 0x0004
    };
    int type() const;

    GGLink actionLink() const;
    void setActionLink(GGLink link);

    bool removeConnection(GGConnection *connection);
    QList<GGConnection *> getConnections() const;

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

protected:
    GGLink m_actionLink;
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGDecisionPage : public GGMappedContentPage
{
public:
    GGDecisionPage(GG::SceneID scene);

    enum {
        Type = 0x0005
    };
    int type() const;

    QList<GGConnection *> getDecisionConnections() const;
    void addDecisionLink(GGLink link);
    bool insertDecisionLink(int idx, GGLink link);
    bool setDecisionLink(int idx, GGLink link);
    bool removeDecisionLink(int idx);
    QList<GGLink> getDecisionLinks() const;

    QList<GGConnection *> getConnections() const;
    bool removeConnection(GGConnection *connection);

    virtual bool match(const GGSearchRequest &req, GGSearchResult &results) const;

protected:
    QList<GGLink> m_decisionLinks;
};

//////////////////////////////////////////

template <class T> inline T ggpage_cast(GGPage *p)
{ return (p && int(static_cast<T>(0)->Type) == p->type()) ? static_cast<T>(p) : 0; }

template <class T> inline T ggpage_cast(const GGPage *p)
{ return (p && int(static_cast<T>(0)->Type) == p->type()) ? static_cast<T>(p) : 0; }

namespace GG {
template <class T> inline T* as(GGPage *p)
{ return ggpage_cast<T*>(p); }

template <class T> inline T* as(const GGPage *p)
{ return ggpage_cast<T*>(p); }

template <> inline const GGContentPage *as(const GGPage *p)
{
    const GGContentPage *cp = ggpage_cast<const GGStartPage*>(p);
    if (!cp) cp = ggpage_cast<const GGEndPage*>(p);
    if (!cp) cp = ggpage_cast<const GGActionPage*>(p);
    if (!cp) cp = ggpage_cast<const GGDecisionPage*>(p);
    return cp;
}

template <> inline GGContentPage *as(GGPage *p) {
    return const_cast<GGContentPage*> (as<const GGContentPage>(const_cast<const GGPage*>(p)));
}

template <> inline const GGEntryActionPage *as(const GGPage *p)
{
    const GGEntryActionPage *eap = ggpage_cast<const GGEndPage*>(p);
    if (!eap) eap = ggpage_cast<const GGActionPage*>(p);
    if (!eap) eap = ggpage_cast<const GGDecisionPage*>(p);
    return eap;
}

template <> inline GGEntryActionPage *as(GGPage *p) {
    return const_cast<GGEntryActionPage*> (as<const GGEntryActionPage>(const_cast<const GGPage*>(p)));
}

template <> inline const GGMappedContentPage *as(const GGPage *p)
{
    const GGMappedContentPage *mcp = ggpage_cast<const GGActionPage*>(p);
    if (!mcp) mcp = ggpage_cast<const GGDecisionPage*>(p);
    return mcp;
}

template <> inline GGMappedContentPage *as(GGPage *p) {
    return const_cast<GGMappedContentPage*> (as<const GGMappedContentPage>(const_cast<const GGPage *>(p)));
}
}

#endif // GGPAGE_H
