#ifndef GGCREATESCENEDLG_H
#define GGCREATESCENEDLG_H

#include <QDialog>

namespace Ui {
class GGCreateSceneDlg;
}

class GGCreateSceneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GGCreateSceneDlg(QWidget *parent = 0);
    ~GGCreateSceneDlg();

    QString sceneName() const;
    QString sceneDir() const;

private slots:
    void checkOk();

    void on_txtSceneName_textEdited();
    void on_txtSceneDir_textEdited();
    void on_txtSceneDir_editingFinished();

private:
    Ui::GGCreateSceneDlg *ui;
    bool m_mediaEdited;
};

#endif // GGCREATESCENEDLG_H
