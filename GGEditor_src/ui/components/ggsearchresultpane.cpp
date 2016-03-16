#include "ggsearchresultpane.h"
#include "ui_ggsearchresultpane.h"
#include <view/ggsearchresultmodel.h>
#include <ui/basic/gghtmlitemdelegate.h>

GGSearchResultPane::GGSearchResultPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGSearchResultPane)
{
    ui->setupUi(this);
    GGSearchResultModel *model = new GGSearchResultModel;
    model->setMatchHighlightDecoration("<span style=\"background-color: rgb(255,255,0);}\">", "</span>");
    ui->lstResults->setModel(model);
    ui->lstResults->setItemDelegateForColumn(0, new GGHtmlItemDelegate);
}

GGSearchResultPane::~GGSearchResultPane()
{
    delete ui;
}

void GGSearchResultPane::setResults(const GGSearchResult &results)
{
    static_cast<GGSearchResultModel*> (ui->lstResults->model())->setSearchResults(results);
    ui->lblResultCount->setText(tr("%n Search Results", NULL, results.count()));
}

void GGSearchResultPane::newSearch()
{
    emit requestNewSearch();
}
