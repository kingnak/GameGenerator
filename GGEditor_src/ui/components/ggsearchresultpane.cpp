#include "ggsearchresultpane.h"
#include "ui_ggsearchresultpane.h"
#include <view/ggsearchresultmodel.h>
#include <ui/basic/gghtmlitemdelegate.h>

GGSearchResultPane::GGSearchResultPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGSearchResultPane)
{
    ui->setupUi(this);
    m_model = new GGSearchResultModel(this);
    m_model->setMatchHighlightDecoration("<span style=\"background-color: rgb(255,255,0);}\">", "</span>", true);
    ui->lstResults->setModel(m_model);
    ui->lstResults->setItemDelegateForColumn(GGSearchResultModel::MATCH_COLUMN, new GGHtmlItemDelegate(this));
    ui->lstResults->setColumnWidth(GGSearchResultModel::WHAT_COLUMN, 20);
    ui->lstResults->setColumnWidth(GGSearchResultModel::WHERE_COLUMN, 20);
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

void GGSearchResultPane::clearResults()
{
    m_model->reset();
}

void GGSearchResultPane::on_lstResults_activated(const QModelIndex &idx)
{
    bool ok;
    GG::PageID id = static_cast<GG::PageID> (ui->lstResults->model()->data(idx, GGSearchResultModel::PageIdRole).toInt(&ok));
    if (ok && id != GG::InvalidPageId) {
        emit highlightPage(id);
    }
}

