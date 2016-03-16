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

private slots:
    void newSearch();

private:
    Ui::GGSearchResultPane *ui;
};

#endif // GGSEARCHRESULTPANE_H
