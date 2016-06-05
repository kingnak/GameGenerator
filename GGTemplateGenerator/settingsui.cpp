#include "settingsui.h"
#include "ui_settingsui.h"

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
