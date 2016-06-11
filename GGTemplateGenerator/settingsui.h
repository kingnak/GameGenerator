#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>

namespace Ui {
class SettingsUI;
}

class GGTemplateGeneratorPlugin;

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

signals:
    void openFileEditor(const QString &file);

private slots:
    void editTemplateByButton();
    void editFile(const QString &path);

private:
    Ui::SettingsUI *ui;
    friend class GGTemplateGeneratorPlugin;
};

#endif // SETTINGSUI_H
