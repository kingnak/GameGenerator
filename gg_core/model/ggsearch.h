#ifndef GGSEARCHREQUEST_H
#define GGSEARCHREQUEST_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>
#include <QRegExp>
#include <QList>
#include <model/ggconnectionslotdata.h>

class GGAbstractModel;

class GG_CORESHARED_EXPORT GGSearchRequest
{
public:
    enum Type {
        CaseInsensitive = 0x00,
        CaseSensitive   = 0x01,
        WholeWord       = 0x02,
        Exact           = 0x04,
        StartsWith      = 0x08
    };
    Q_DECLARE_FLAGS(Types, Type)

    enum What {
        Nothing         = 0x0000,
        PageName        = 0x0001,
        PageCaption     = 0x0004,
        PageContent     = 0x0008,
        Variable        = 0x0010,
        Function        = 0x0020,
        LinkName        = 0x0040,
        //Other         = 0x0080,
        All = PageName | PageCaption | PageContent | Variable | Function | LinkName // | Other
    };
    Q_DECLARE_FLAGS(Whats, What)

    GGSearchRequest(const QString &term = QString::null, int type = CaseInsensitive, int what = All);

    void setScene(GG::SceneID id) { m_scene = id; }
    GG::SceneID scene() const { return m_scene; }

    void setMaxResults(int maxResults) { m_max = maxResults; }
    int maxResults() const { return m_max; }

    void setType(Types type);
    Types type() const { return m_type; }

    void setWhat(Whats what) { m_what = what; }
    Whats what() const { return m_what; }

    void setTerm(const QString &term);
    QString term() const { return m_term; }

    bool matches(const QString &str) const;
    bool searchLinks() const;

    QString formatMatch(const QString &str, const QString &preFix, const QString &postFix) const;

private:
    Types m_type;
    Whats m_what;
    int m_max;
    GG::SceneID m_scene;
    QString m_term;
    QRegExp m_matcher;
};

//////////////////////////////

class GG_CORESHARED_EXPORT GGSearchResultItem
{
public:
    enum Where {
        Nowhere,
        PageName, PageScene, PageCaption,
        // TODO: Contents
        LinkName, Action, Condition,
        LinkAction,
        Definition
    };

    enum Probability {
        Impossible, Certain, Probable, Possible
    };

    GGSearchResultItem() :
        m_where(Nowhere), m_what(GGSearchRequest::Nothing), m_pageId(GG::InvalidPageId), m_slot(GGConnectionSlotData::NoConnection), m_prob(Impossible)
    {}

    GGSearchResultItem(Where where, GGSearchRequest::What what, const QString &match, GG::PageID pageId = GG::InvalidPageId, GGConnectionSlotData slot = GGConnectionSlotData::NoConnection, Probability prob = Certain)
        : m_where(where), m_what(what), m_match(match), m_pageId(pageId), m_slot(slot), m_prob(prob)
    {}

    Where where() const { return m_where; }
    GGSearchRequest::What what() const { return m_what; }
    QString matchString() const { return m_match; }

    GG::PageID pageId() const { return m_pageId; }
    GGConnectionSlotData slot() const { return m_slot; }

    Probability probability() const { return m_prob; }

    bool isValid() const { return m_prob != Impossible && m_where != Nowhere && m_what != GGSearchRequest::Nothing; }

private:
    Where m_where;
    GGSearchRequest::What m_what;
    QString m_match;
    GG::PageID m_pageId;
    GGConnectionSlotData m_slot;
    Probability m_prob;
};

class GG_CORESHARED_EXPORT GGSearchResult
{
public:
    GGSearchResult() : m_model(NULL) {}
    GGSearchResult(const GGSearchRequest &req, const GGAbstractModel *model) : m_model(model), m_req(req) {}

    QList<GGSearchResultItem> resultItems() { return m_items; }
    GGSearchRequest request() const { return m_req; }
    const GGAbstractModel *model() const { return m_model; }
    int count() const { return m_items.size(); }

    void append(const GGSearchResultItem &item) { m_items.append(item); }

    GGSearchResult &operator << (const GGSearchResultItem &item) {
        m_items << item;
        return *this;
    }

    const GGSearchResultItem &operator[] (int idx) const {
        return m_items[idx];
    }

private:
    QList<GGSearchResultItem> m_items;
    const GGAbstractModel *m_model;
    GGSearchRequest m_req;
};

#endif // GGSEARCHREQUEST_H
