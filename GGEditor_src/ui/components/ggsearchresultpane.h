#ifndef GGSEARCHRESULTPANE_H
#define GGSEARCHRESULTPANE_H

#include <QWidget>
#include <model/ggsearch.h>

namespace Ui {
class GGSearchResultPane;
}

class GGSearchResultModel;

class GGSearchResultPane : public QWidget
{
    Q_OBJECT

public:
    explicit GGSearchResultPane(QWidget *parent = 0);
    ~GGSearchResultPane();

public slots:
    void setResults(const GGSearchResult &results);

signals:
    void requestNewSearch();
    void highlightPage(GG::PageID id);

private slots:
    void newSearch();
    void on_lstResults_activated(const QModelIndex &idx);

private:
    Ui::GGSearchResultPane *ui;
};

#endif // GGSEARCHRESULTPANE_H
