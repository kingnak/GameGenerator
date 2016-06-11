#include "settingsui.h"
#include "ui_settingsui.h"
#include <QFile>

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);
}

SettingsUI::~SettingsUI()
{
    delete ui;
}

void SettingsUI::editTemplateByButton()
{
    QString name = sender()->property("TEMPLATE").toString();
    QLineEdit *l = this->findChild<QLineEdit*>(name);
    if (l) {
        editFile(l->text());
    }
}

void SettingsUI::editFile(const QString &path)
{
    if (!QFile::exists(path)) {
        return;
    }

    emit openFileEditor(path);
}
