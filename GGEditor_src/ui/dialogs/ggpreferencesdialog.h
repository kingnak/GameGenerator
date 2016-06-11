#ifndef GGPREFERENCESDIALOG_H
#define GGPREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class GGPreferencesDialog;
}

class GGPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGPreferencesDialog(QWidget *parent = 0);
    ~GGPreferencesDialog();

public slots:
    void accept();

private slots:
    void on_btnBrowseEditor_clicked();

private:
    Ui::GGPreferencesDialog *ui;
};

#endif // GGPREFERENCESDIALOG_H
