#include "ggcreateprojectdialog.h"
#include "ui_ggcreateprojectdialog.h"
#include <QtWidgets>

GGCreateProjectDialog::GGCreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGCreateProjectDialog)
{
    ui->setupUi(this);
}

GGCreateProjectDialog::~GGCreateProjectDialog()
{
    delete ui;
}

QString GGCreateProjectDialog::projectBasePath() const
{
    return ui->txtPath->text();
}

QString GGCreateProjectDialog::projectTitle() const
{
    return ui->txtTitle->text();
}

void GGCreateProjectDialog::on_btnBrowse_clicked()
{
    QString s = QFileDialog::getExistingDirectory(this, "Project base path", ui->txtPath->text());
    if (!s.isNull()) {
        ui->txtPath->setText(s);
    }
    checkOk();
}

void GGCreateProjectDialog::checkOk()
{
    bool ok = !ui->txtTitle->text().isEmpty();
    ok &= !ui->txtPath->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

