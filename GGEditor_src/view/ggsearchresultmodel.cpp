#include "ggsearchresultmodel.h"

GGSearchResultModel::GGSearchResultModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

QVariant GGSearchResultModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    if (index.row() < 0 || index.row() >= m_results.count()) return QVariant();
    switch (index.column()) {
    case 0:
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
    case 0:
        return "Match";
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
    return 1;
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

