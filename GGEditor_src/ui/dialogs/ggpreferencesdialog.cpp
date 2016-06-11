#include "ggpreferencesdialog.h"
#include "ui_ggpreferencesdialog.h"
#include <QFileDialog>
#include <utils/ggglobaluserinfo.h>
#include <io/ggglobalsettingsserializer.h>

GGPreferencesDialog::GGPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGPreferencesDialog)
{
    ui->setupUi(this);
    ui->txtExternalEditor->setText(QDir::toNativeSeparators(GGGlobalUserInfo::instance().externalEditor()));
}

GGPreferencesDialog::~GGPreferencesDialog()
{
    delete ui;
}

void GGPreferencesDialog::accept()
{
    GGGlobalUserInfo::instance().setExternalEditor(ui->txtExternalEditor->text());
    GGGlobalSettingsSerializer::saveDefaultFile(&GGGlobalUserInfo::instance());
    QDialog::accept();
}

void GGPreferencesDialog::on_btnBrowseEditor_clicked()
{
    QString path;
    if (ui->txtExternalEditor->text().isEmpty()) {
        path = QDir::homePath();
    } else {
        QFileInfo fi(ui->txtExternalEditor->text());
        path = fi.absolutePath();
    }
    QString s = QFileDialog::getOpenFileName(this, "External Editor", path, "*.exe");
    if (!s.isNull()) {
        ui->txtExternalEditor->setText(QDir::toNativeSeparators(s));
    }
}
