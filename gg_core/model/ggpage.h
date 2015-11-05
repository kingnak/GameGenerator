#ifndef GGPAGE_H
#define GGPAGE_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>

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

    void setSceneName(QString sn) { m_sceneName = sn; }
    void setName(QString n) { m_name = n; }

    virtual QList<GGConnection *> getConnections() = 0;
    virtual bool removeConnection(GGConnection *connection) = 0;

private:
    GG::PageID m_id;
    GGAbstractModel *m_model;

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


    GGConnection *trueConnection();
    GGConnection *falseConnection();

    void setTrueConnection(GGConnection *t);
    void setFalseConnection(GGConnection *f);

    QList<GGConnection *> getConnections();
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

    GGContentElement *content();
    void setContent(GGContentElement *cont);

protected:
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

    void setConnection(GGConnection *conn);
    QList<GGConnection *> getConnections();
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

    QList<GGConnection *> getConnections();
    bool removeConnection(GGConnection *connection);
};

#endif // GGPAGE_H
