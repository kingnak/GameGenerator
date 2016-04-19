#include "ggcreatescenedlg.h"
#include "ui_ggcreatescenedlg.h"
#include <QPushButton>
#include <ggutilities.h>

GGCreateSceneDlg::GGCreateSceneDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGCreateSceneDlg),
    m_mediaEdited(false)
{
    ui->setupUi(this);
    checkOk();
}

GGCreateSceneDlg::~GGCreateSceneDlg()
{
    delete ui;
}

QString GGCreateSceneDlg::sceneName() const
{
    return ui->txtSceneName->text();
}

QString GGCreateSceneDlg::sceneDir() const
{
    return ui->txtSceneDir->text();
}

void GGCreateSceneDlg::checkOk()
{
    bool ok = !ui->txtSceneName->text().isEmpty();
    ok &= !ui->txtSceneDir->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

void GGCreateSceneDlg::on_txtSceneName_textEdited()
{
    if (!m_mediaEdited) {
        ui->txtSceneDir->setText(GGUtilities::sanatizeFileName(ui->txtSceneName->text()));
    }
}

void GGCreateSceneDlg::on_txtSceneDir_textEdited()
{
    m_mediaEdited = true;
}

void GGCreateSceneDlg::on_txtSceneDir_editingFinished()
{
    if (ui->txtSceneDir->text().isEmpty()) {
        m_mediaEdited = false;
        on_txtSceneName_textEdited();
    }
}
