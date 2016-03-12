#include "ggsearch.h"

GGSearchRequest::GGSearchRequest(const QString &term, int type, int what)
    : m_type(type), m_what(what), m_max(-1)
{
    setTerm(term);
}

void GGSearchRequest::setType(Types type)
{
    m_type = type;
    setTerm(m_term);
}

void GGSearchRequest::setTerm(const QString &term)
{
    m_term = term;
    QString cp = term;
    if (m_type.testFlag(Exact) || m_type.testFlag(StartsWith)) {
        return;
    }

    if (m_type.testFlag(WholeWord)) {
        cp = QString("\\b%1\\b").arg(cp);
    }
    m_matcher = QRegExp(cp);
    if (m_type.testFlag(CaseSensitive)) {
        m_matcher.setCaseSensitivity(Qt::CaseSensitive);
    } else {
        m_matcher.setCaseSensitivity(Qt::CaseInsensitive);
    }
}

bool GGSearchRequest::matches(const QString &str) const
{
    Qt::CaseSensitivity cs = m_type.testFlag(CaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;
    if (m_type.testFlag(Exact)) {
        return str.compare(m_term, cs) == 0;
    }
    if (m_type.testFlag(StartsWith)) {
        return str.startsWith(m_term, cs);
    }
    return m_matcher.indexIn(str) >= 0;
}

bool GGSearchRequest::searchLinks() const
{
    return (m_what & (Variable | Function | LinkName)) != 0;
}
