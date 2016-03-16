#include "ggsearchdialog.h"
#include "ui_ggsearchdialog.h"

GGSearchDialog::GGSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGSearchDialog)
{
    ui->setupUi(this);
    resetSearch();
}

GGSearchDialog::~GGSearchDialog()
{
    delete ui;
}

void GGSearchDialog::accept()
{
    if (updateSearch()) {

        emit executeSearch(m_lastSearch);
    }
}

void GGSearchDialog::resetSearch()
{
    ui->chkEverything->setChecked(false);
    ui->chkEverything->toggle();
    ui->cmbSearch->setEditText("");
    ui->radContains->setChecked(true);
    ui->chkCaseSensitive->setChecked(false);
    ui->cmbSearch->setFocus();
}

bool GGSearchDialog::updateSearch()
{
    if (ui->cmbSearch->currentText().trimmed().isEmpty()) {
        return false;
    }
    GGSearchRequest req;
    req.setTerm(ui->cmbSearch->currentText());
    if (ui->chkEverything->isChecked()) {
        req.setWhat(GGSearchRequest::All);
    } else {
        GGSearchRequest::Whats what(GGSearchRequest::Nothing);
        if (ui->chkPageCaption->isChecked()) what |= GGSearchRequest::PageCaption;
        if (ui->chkPageScene->isChecked()) what |= GGSearchRequest::PageScene;
        if (ui->chkPageName->isChecked()) what |= GGSearchRequest::PageName;
        if (ui->chkLinkName->isChecked()) what |= GGSearchRequest::LinkName;
        if (ui->chkContent->isChecked()) what |= GGSearchRequest::PageContent;
        if (ui->chkVariables->isChecked()) what |= GGSearchRequest::Variable;
        if (ui->chkFunctions->isChecked()) what |= GGSearchRequest::Function;
        req.setWhat(what);
    }

    GGSearchRequest::Types type(GGSearchRequest::CaseInsensitive);
    if (ui->chkCaseSensitive->isChecked()) type |= GGSearchRequest::CaseSensitive;
    if (ui->radExact->isChecked()) type |= GGSearchRequest::Exact;
    else if (ui->radStartsWith->isChecked()) type |= GGSearchRequest::StartsWith;
    else if (ui->radWholeWord->isChecked()) type |= GGSearchRequest::WholeWord;
    req.setType(type);

    m_lastSearch = req;
    return true;
}
