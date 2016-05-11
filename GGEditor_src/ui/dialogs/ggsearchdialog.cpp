#include "ggsearchdialog.h"
#include "ui_ggsearchdialog.h"

GGSearchDialog::GGSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGSearchDialog),
    m_currentSceneId(GG::InvalidSceneId)
{
    ui->setupUi(this);
    resetSearch();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setAutoDefault(true);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(false);
}

GGSearchDialog::~GGSearchDialog()
{
    delete ui;
}

void GGSearchDialog::setCurrentSceneId(GG::SceneID id)
{
    m_currentSceneId = id;
    // TODO: Set "Current Scene" disabled? => How?
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
    ui->cmbWhere->setCurrentIndex(0);
    ui->cmbSearch->setFocus();
}

bool GGSearchDialog::updateSearch()
{
    if (ui->cmbSearch->currentText().trimmed().isEmpty()) {
        return false;
    }
    GGSearchRequest req;
    req.setTerm(ui->cmbSearch->currentText());

    if (ui->cmbWhere->currentIndex() == 1)
        req.setScene(m_currentSceneId);
    else
        req.setScene(GG::InvalidSceneId);

    if (ui->chkEverything->isChecked()) {
        req.setWhat(GGSearchRequest::All);
    } else {
        GGSearchRequest::Whats what(GGSearchRequest::Nothing);
        if (ui->chkPageCaption->isChecked()) what |= GGSearchRequest::PageCaption;
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
