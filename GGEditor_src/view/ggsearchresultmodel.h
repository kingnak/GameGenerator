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

    enum Columns {
        WHAT_COLUMN = 0,
        WHERE_COLUMN = 1,
        PAGE_COLUMN = 2,
        MATCH_COLUMN = 3
    };

    enum {
        PageIdRole = Qt::UserRole + 1
    };


signals:

public slots:
    void setSearchResults(const GGSearchResult &results);

private:
    static QVariant getWhatData(GGSearchRequest::What what, int role);

private:
    GGSearchResult m_results;
    QString m_highPre;
    QString m_highPost;

    static const int COLUMN_COUNT = 4;
};

#endif // GGSEARCHRESULTMODEL_H
