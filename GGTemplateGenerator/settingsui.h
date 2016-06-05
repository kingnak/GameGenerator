#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

private:
    Ui::SettingsUI *ui;
};

#endif // SETTINGSUI_H
