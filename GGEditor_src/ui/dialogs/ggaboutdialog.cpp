#include "ggaboutdialog.h"
#include "ui_ggaboutdialog.h"
#include <gg_definitions.h>
#include <ggutilities.h>

GGAboutDialog::GGAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGAboutDialog)
{
    ui->setupUi(this);
    ui->textBrowser->setHtml(ui->textBrowser->toHtml().arg(GGUtilities::versionToString(GG::VERSION)));
}

GGAboutDialog::~GGAboutDialog()
{
    delete ui;
}
