#include "ggcreateprojectdialog.h"
#include "ui_ggcreateprojectdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <ggutilities.h>

GGCreateProjectDialog::GGCreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGCreateProjectDialog),
    m_mediaEdited(false)
{
    ui->setupUi(this);
    checkOk();
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

QString GGCreateProjectDialog::initialSceneName() const
{
    return ui->txtDefaultSceneName->text();
}

void GGCreateProjectDialog::accept()
{
    QDir d(ui->txtPath->text());
    QString path = QDir::toNativeSeparators(d.absolutePath());
    ui->txtPath->setText(path);

    if (d.exists()) {
        QStringList lst = d.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        if (!lst.isEmpty()) {

            int res = QMessageBox::warning(this, "Create Project", QString("The folder\n%1\nis not empty.\n\nAre you sure you want to create a project there?")
                                           .arg(path), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (res == QMessageBox::No) {
                return;
            }
        }
    } else {
        if (!d.mkpath(".")) {
            QMessageBox::critical(this, "Create Project", QString("Cannot create folder\n%1").arg(path));
            return;
        }
    }

    return QDialog::accept();
}

void GGCreateProjectDialog::on_btnBrowse_clicked()
{
    QString s = QFileDialog::getExistingDirectory(this, "Project base path", ui->txtPath->text());
    if (!s.isNull()) {
        ui->txtPath->setText(s);
    }
    checkOk();
}

void GGCreateProjectDialog::on_txtDefaultSceneName_textEdited()
{
    if (!m_mediaEdited) {
        ui->txtDefaultSceneDir->setText(GGUtilities::sanatizeFileName(ui->txtDefaultSceneName->text()));
    }
}

void GGCreateProjectDialog::on_txtDefaultSceneDir_textEdited()
{
    if (ui->txtDefaultSceneDir->text().isEmpty()) {
        m_mediaEdited = false;
    } else {
        m_mediaEdited = true;
    }
}

void GGCreateProjectDialog::on_txtDefaultSceneDir_editingFinished()
{
    ui->txtDefaultSceneDir->setText(GGUtilities::sanatizeFileName(ui->txtDefaultSceneDir->text()));
    if (ui->txtDefaultSceneDir->text().isEmpty()) {
        ui->txtDefaultSceneDir->setText(GGUtilities::sanatizeFileName(ui->txtDefaultSceneName->text()));
    }
}

void GGCreateProjectDialog::checkOk()
{
    bool ok = !ui->txtTitle->text().isEmpty();
    ok &= !ui->txtPath->text().isEmpty();
    ok &= !ui->txtDefaultSceneName->text().isEmpty();
    ok &= !ui->txtDefaultSceneDir->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
