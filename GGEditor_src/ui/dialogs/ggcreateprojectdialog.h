#ifndef GGCREATEPROJECTDIALOG_H
#define GGCREATEPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class GGCreateProjectDialog;
}

class GGCreateProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGCreateProjectDialog(QWidget *parent = 0);
    ~GGCreateProjectDialog();

    QString projectBasePath() const;
    QString projectTitle() const;

public slots:
    void accept();

private slots:
    void on_btnBrowse_clicked();

    void checkOk();

private:
    Ui::GGCreateProjectDialog *ui;
};

#endif // GGCREATEPROJECTDIALOG_H
