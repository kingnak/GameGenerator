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
    QString initialSceneName() const;
    QString initialSceneDir() const;

public slots:
    void accept();

private slots:
    void on_btnBrowse_clicked();
    void on_txtDefaultSceneName_textEdited();
    void on_txtDefaultSceneDir_textEdited();
    void on_txtDefaultSceneDir_editingFinished();

    void checkOk();

private:
    Ui::GGCreateProjectDialog *ui;

    bool m_mediaEdited;
};

#endif // GGCREATEPROJECTDIALOG_H
