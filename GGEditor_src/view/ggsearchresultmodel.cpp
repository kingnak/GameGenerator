#include "ggsearchresultmodel.h"
#include <model/ggabstractmodel.h>
#include <model/ggpage.h>
#include <QIcon>

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

    switch (index.column()) {
    case WHAT_COLUMN:
        return getWhatData(m_results[index.row()].what(), role);

    case WHERE_COLUMN:
        return getWhereData(m_results[index.row()].where(), role);

    case PAGE_COLUMN:
        if (role == Qt::DisplayRole) {
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
        }

    case MATCH_COLUMN:
        if (role == Qt::DisplayRole)
        {
            if (m_highPre.isEmpty() && m_highPost.isEmpty()) {
                return m_results[index.row()].matchString();
            } else {
                QString match = m_results[index.row()].matchString();
                return m_results.request().formatMatch(match, m_highPre, m_highPost);
            }
        }
    }
    return QVariant();
}

QVariant GGSearchResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    switch (section) {
    case WHAT_COLUMN: if (role == Qt::DisplayRole) return "T"; else if (role == Qt::ToolTipRole) return "Type"; break;
    case WHERE_COLUMN: if (role == Qt::DisplayRole) return "W"; else if (role == Qt::ToolTipRole) return "Where"; break;
    case PAGE_COLUMN: if (role == Qt::DisplayRole) return "Page"; break;
    case MATCH_COLUMN: if (role == Qt::DisplayRole) return "Match"; break;
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

void GGSearchResultModel::reset()
{
    setSearchResults(GGSearchResult());
}

QVariant GGSearchResultModel::getWhatData(GGSearchRequest::What what, int role)
{
    switch (what) {
    case GGSearchRequest::PageName:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/page");
        if (role == Qt::ToolTipRole)
            return "Page Name";
        break;
    case GGSearchRequest::PageCaption:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/title");
        if (role == Qt::ToolTipRole)
            return "Page Caption";
        break;
    case GGSearchRequest::PageContent:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/content");
        if (role == Qt::ToolTipRole)
            return "Page Content";
        break;
    case GGSearchRequest::Variable:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/variable");
        if (role == Qt::ToolTipRole)
            return "Variable";
        break;
    case GGSearchRequest::Function:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/function");
        if (role == Qt::ToolTipRole)
            return "Function";
        break;
    case GGSearchRequest::LinkName:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/link");
        if (role == Qt::ToolTipRole)
                return "Link";
        break;
    default:
        if (role == Qt::DecorationRole)
            return QIcon(":/search/other");
        if (role == Qt::ToolTipRole)
            return "Other";
        break;
    }
    return QVariant();
}

QVariant GGSearchResultModel::getWhereData(GGSearchResultItem::Where where, int role)
{
    switch (where) {
    //case GGSearchResultItem::PageName: return "N";
    //case GGSearchResultItem::PageScene: return "S";
    //case GGSearchResultItem::PageCaption: return "T";
    case GGSearchResultItem::LinkAction:
        if (role == Qt::DecorationRole)
            return QPixmap(":/search/link");
        if (role == Qt::ToolTipRole)
            return "In Link Action";
        break;
    case GGSearchResultItem::Action:
        if (role == Qt::DecorationRole)
            return QPixmap(":/search/actionpage");
        if (role == Qt::ToolTipRole)
            return "In Page Action";
        break;
    case GGSearchResultItem::Condition:
        if (role == Qt::DecorationRole)
            return QPixmap(":/search/condition");
        if (role == Qt::ToolTipRole)
            return "In Condition";
        break;
    case GGSearchResultItem::Definition:
        if (role == Qt::DecorationRole)
            return QPixmap(":/search/definition");
        if (role == Qt::ToolTipRole)
            return "In Definition";
        break;
    default:
        if (role == Qt::DecorationRole)
            return QPixmap(":/search/other");
        if (role == Qt::ToolTipRole)
            return "Elsewhere";
        break;
    }
    return QVariant();
}

