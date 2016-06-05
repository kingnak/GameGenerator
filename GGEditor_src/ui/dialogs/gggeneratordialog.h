#ifndef GGGENERATORDIALOG_H
#define GGGENERATORDIALOG_H

#include <QDialog>

class GGGeneratorManager;
class GGGeneratorInterface;
class GGEditProject;
class QDir;

namespace Ui {
class GGGeneratorDialog;
}

class GGGeneratorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GGGeneratorDialog(GGEditProject *project, const QDir &pluginDir, QWidget *parent = 0);
    ~GGGeneratorDialog();

public slots:
    void enableGeneration(bool enable = true);
    void disableGeneration(bool disable = true);
    void accept();
    void reject();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void generate();
    void loadGenerators(const QDir &generatorsDir, const QDir &pluginDir);

    void showGenerator(GGGeneratorInterface *generator);

    bool saveChanges();

    void on_btnBrowse_clicked();

private:
    GGGeneratorInterface *currentGenerator();
    void cleanUpGenerators();

private:
    Ui::GGGeneratorDialog *ui;
    GGEditProject *m_project;
    QPushButton *m_btnGenerate;
    GGGeneratorManager *m_manager;
    GGGeneratorInterface *m_curGenerator;
};

#endif // GGGENERATORDIALOG_H
