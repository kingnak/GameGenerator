#include "ggsearchresultmodel.h"
#include <model/ggabstractmodel.h>
#include <model/ggpage.h>

GGSearchResultModel::GGSearchResultModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

QVariant GGSearchResultModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_results.count()) return QVariant();

    if (role == PageIdRole) {
        return m_results[index.row()].pageId();
    }

    if (role != Qt::DisplayRole) return QVariant();
    switch (index.column()) {
    case WHAT_COLUMN:
        switch (m_results[index.row()].what()) {
        case GGSearchRequest::PageName: return "N";
        case GGSearchRequest::PageScene: return "S";
        case GGSearchRequest::PageCaption: return "T";
        case GGSearchRequest::PageContent: return "C";
        case GGSearchRequest::Variable: return "V";
        case GGSearchRequest::Function: return "F";
        case GGSearchRequest::LinkName: return "L";
        default: return "O";
        }
        break;

    case WHERE_COLUMN:
        switch (m_results[index.row()].where()) {
        case GGSearchResultItem::PageName: return "N";
        case GGSearchResultItem::PageScene: return "S";
        case GGSearchResultItem::PageCaption: return "T";
        case GGSearchResultItem::LinkName: return "L";
        case GGSearchResultItem::Action: return "A";
        case GGSearchResultItem::Condition: return "C";
        case GGSearchResultItem::Definition: return "D";
        default: return "O";
        }
        break;

    case PAGE_COLUMN:
        if (m_results[index.row()].pageId() != GG::InvalidPageId) {
            QString name = m_results.model()->getPage(m_results[index.row()].pageId())->name();
            if (name.isEmpty()) {
                return QString("{Page %1}").arg(m_results[index.row()].pageId());
            } else {
                return name;
            }
        } else {
            return "";
        }

    case MATCH_COLUMN:
        if (m_highPre.isEmpty() && m_highPost.isEmpty()) {
            return m_results[index.row()].matchString();
        } else {
            QString match = m_results[index.row()].matchString();
            return m_results.request().formatMatch(match, m_highPre, m_highPost);
        }
    }
    return QVariant();
}

QVariant GGSearchResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case WHAT_COLUMN: return "T";
    case WHERE_COLUMN: return "W";
    case PAGE_COLUMN: return "Page";
    case MATCH_COLUMN: return "Match";
    }
    return QVariant();
}

int GGSearchResultModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_results.count();
}

int GGSearchResultModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

void GGSearchResultModel::setMatchHighlightDecoration(const QString &preFix, const QString &postFix)
{
    m_highPre = preFix;
    m_highPost = postFix;
}

void GGSearchResultModel::setSearchResults(const GGSearchResult &results)
{
    beginResetModel();
    m_results = results;
    endResetModel();
}

