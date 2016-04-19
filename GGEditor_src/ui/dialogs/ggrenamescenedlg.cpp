#include "ggrenamescenedlg.h"
#include "ui_ggrenamescenedlg.h"
#include <QPushButton>
#include <model/ggscene.h>
#include <ggutilities.h>

GGRenameSceneDlg::GGRenameSceneDlg(GGScene *scene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGRenameSceneDlg),
    m_mediaModified(false)
{
    ui->setupUi(this);
    m_name = scene->name();
    m_media = scene->mediaDir();

    ui->txtMediaDir->setText(m_media);
    ui->txtSceneName->setText(m_name);
}

GGRenameSceneDlg::~GGRenameSceneDlg()
{
    delete ui;
}

QString GGRenameSceneDlg::newName() const
{
    return ui->txtSceneName->text();
}

bool GGRenameSceneDlg::renameDir() const
{
    return ui->chkRenameMediaDir->isChecked();
}

QString GGRenameSceneDlg::newMedia() const
{
    return ui->txtMediaDir->text();
}

void GGRenameSceneDlg::sceneNameEdited(const QString &name)
{
    if (ui->chkRenameMediaDir->isChecked()) {
        if (!m_mediaModified) {
            ui->txtMediaDir->setText(GGUtilities::sanatizeFileName(name));
        }
    }
}

void GGRenameSceneDlg::on_txtMediaDir_textEdited()
{
    m_mediaModified = true;
}

void GGRenameSceneDlg::on_txtMediaDir_editingFinished()
{
    ui->txtMediaDir->setText(GGUtilities::sanatizeFileName(ui->txtMediaDir->text()));
    if (ui->txtMediaDir->text().isEmpty()) {
        ui->txtMediaDir->setText(GGUtilities::sanatizeFileName(ui->txtSceneName->text()));
    }
}

void GGRenameSceneDlg::on_chkRenameMediaDir_toggled(bool on)
{
    if (on) {
        ui->txtMediaDir->setText(GGUtilities::sanatizeFileName(ui->txtSceneName->text()));
    } else {
        ui->txtMediaDir->setText(m_media);
    }
    m_mediaModified = false;
}

void GGRenameSceneDlg::checkOk()
{
    bool ok = !ui->txtSceneName->text().isEmpty();
    if (ui->chkRenameMediaDir->isChecked()) {
        ok &= !ui->txtMediaDir->text().isEmpty();
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
