#ifndef GGPAGE_H
#define GGPAGE_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>
#include "ggmappedlink.h"
#include "gglink.h"

class GGConnection;
class GGAbstractModel;
class GGContentElement;

class GG_CORESHARED_EXPORT GGPage
{
    Q_DISABLE_COPY(GGPage)

protected:
    GGPage();

public:
    virtual ~GGPage();

    virtual int type() const = 0;

    GG::PageID id() const { return m_id; }
    QString sceneName() const { return m_sceneName; }
    QString name() const { return m_name; }
    GGAbstractModel *model() const { return m_model; }

    void setSceneName(QString sn);
    void setName(QString n);

    // TODO: Entry Action (as own class between contentPage and mappedContenPage?)

    virtual QList<GGConnection *> getConnections() const = 0;
    virtual bool removeConnection(GGConnection *connection) = 0;

private:
    GG::PageID m_id;
    GGAbstractModel *m_model;

protected:
    void notifyChanged();

protected:
    QString m_sceneName;
    QString m_name;

    friend class GGAbstractModel;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGConditionPage : public GGPage
{
public:
    GGConditionPage();

    enum {
        Type = 0x0003
    };
    int type() const;

    GGConnection *trueConnection() const;
    GGConnection *falseConnection() const;

    void setTrueConnection(GGConnection *t);
    void setFalseConnection(GGConnection *f);

    // TODO: Condition

    QList<GGConnection *> getConnections() const;
    bool removeConnection(GGConnection *connection);

private:
    GGConnection *m_true;
    GGConnection *m_false;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGContentPage : public GGPage
{
public:
    GGContentPage();
    ~GGContentPage();

    QString caption() const { return m_caption; }
    void setCaption(QString cap);

    GGContentElement *content();
    void setContent(GGContentElement *cont);
    GGContentElement *exchangeContent(GGContentElement *cont);

protected:
    QString m_caption;
    GGContentElement *m_content;
};

/////////////////////////////////////////

class GG_CORESHARED_EXPORT GGStartPage : public GGContentPage
{
public:
    GGStartPage();

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

class GG_CORESHARED_EXPORT GGEndPage : public GGContentPage
{
public:
    GGEndPage();

    enum {
        Type = 0x0002
    };
    int type() const;

    QList<GGConnection *> getConnections() const;
    bool removeConnection(GGConnection *connection);
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGMappedContentPage : public GGContentPage
{
public:
    GGMappedContentPage();

    bool removeConnection(GGConnection *connection);

    QList<GGConnection *> getMappedConnections() const;
    void addMappedLink(GGMappedLink link);
    bool insertMappedLink(int idx, GGMappedLink link);
    bool setMappedLink(int idx, GGMappedLink link);
    bool removeMappedLink(int idx);
    QList<GGMappedLink> getLinkMap() const;

protected:
    QList<GGMappedLink> m_mappedLinks;
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGActionPage : public GGMappedContentPage
{
public:
    GGActionPage();

    enum {
        Type = 0x0004
    };
    int type() const;

    GGLink actionLink() const;
    void setActionLink(GGLink link);

    bool removeConnection(GGConnection *connection);
    QList<GGConnection *> getConnections() const;

protected:
    GGLink m_actionLink;
};

//////////////////////////////////////////

class GG_CORESHARED_EXPORT GGDecisionPage : public GGMappedContentPage
{
public:
    GGDecisionPage();

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

template <> inline GGContentPage *as(GGPage *p)
{
    GGContentPage *cp = ggpage_cast<GGStartPage*>(p);
    if (!cp) cp = ggpage_cast<GGEndPage*>(p);
    if (!cp) cp = ggpage_cast<GGActionPage*>(p);
    if (!cp) cp = ggpage_cast<GGDecisionPage*>(p);
    return cp;
}

template <> inline GGMappedContentPage *as(GGPage *p)
{
    GGMappedContentPage *mcp = ggpage_cast<GGActionPage*>(p);
    if (!mcp) mcp = ggpage_cast<GGDecisionPage*>(p);
    return mcp;
}
}

#endif // GGPAGE_H
