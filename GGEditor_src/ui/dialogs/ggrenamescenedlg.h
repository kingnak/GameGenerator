#ifndef GGRENAMESCENEDLG_H
#define GGRENAMESCENEDLG_H

#include <QDialog>

class GGScene;

namespace Ui {
class GGRenameSceneDlg;
}

class GGRenameSceneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GGRenameSceneDlg(GGScene *scene, QWidget *parent = 0);
    ~GGRenameSceneDlg();

    QString newName() const;
    bool renameDir() const;
    QString newMedia() const;

private slots:
    void sceneNameEdited(const QString &name);
    void on_txtMediaDir_textEdited();
    void on_txtMediaDir_editingFinished();
    void on_chkRenameMediaDir_toggled(bool on);
    void checkOk();

private:
    Ui::GGRenameSceneDlg *ui;
    QString m_name;
    QString m_media;
    bool m_mediaModified;
};

#endif // GGRENAMESCENEDLG_H
