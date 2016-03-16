#ifndef GGSEARCHRESULTMODEL_H
#define GGSEARCHRESULTMODEL_H

#include <QAbstractListModel>
#include <model/ggsearch.h>

class GGSearchResultModel : public QAbstractListModel
{
    Q_OBJECT
public:
    GGSearchResultModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    void setMatchHighlightDecoration(const QString &preFix, const QString &postFix);

signals:

public slots:
    void setSearchResults(const GGSearchResult &results);

private:
    GGSearchResult m_results;
    QString m_highPre;
    QString m_highPost;
};

#endif // GGSEARCHRESULTMODEL_H
