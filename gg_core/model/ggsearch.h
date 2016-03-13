#ifndef GGSEARCHREQUEST_H
#define GGSEARCHREQUEST_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QString>
#include <QRegExp>
#include <QList>
#include <model/ggconnectionslotdata.h>

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
        PageScene       = 0x0002,
        PageCaption     = 0x0004,
        PageContent     = 0x0008,
        Variable        = 0x0010,
        Function        = 0x0020,
        LinkName        = 0x0040,
        All = PageName | PageScene | PageCaption | PageContent | Variable | Function | LinkName
    };
    Q_DECLARE_FLAGS(Whats, What)

    GGSearchRequest(const QString &term, int type = CaseInsensitive, int what = All);

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

private:
    Types m_type;
    Whats m_what;
    int m_max;
    QString m_term;
    QRegExp m_matcher;
};

//////////////////////////////

class GG_CORESHARED_EXPORT GGSearchResult
{
public:
    enum Where {
        Nowhere,
        PageName, PageScene, PageCaption,
        // TODO: Contents
        LinkName, Action, Condition,
        Definition
    };

    enum Probability {
        Impossible, Certain, Probable, Possible
    };

    GGSearchResult() :
        m_where(Nowhere), m_what(GGSearchRequest::Nothing), m_pageId(GG::InvalidPageId), m_slot(GGConnectionSlotData::NoConnection), m_prob(Impossible)
    {}

    GGSearchResult(Where where, GGSearchRequest::What what, const QString &match, GG::PageID pageId = GG::InvalidPageId, GGConnectionSlotData slot = GGConnectionSlotData::NoConnection, Probability prob = Certain)
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

typedef QList<GGSearchResult> GGSearchResultList;

#endif // GGSEARCHREQUEST_H