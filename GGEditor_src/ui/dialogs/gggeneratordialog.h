#ifndef GGGENERATORDIALOG_H
#define GGGENERATORDIALOG_H

#include <QDialog>
#include <QMap>
#include <generator/gggeneratorinterface.h>

class GGGeneratorManager;
class GGEditProject;
class QDir;

namespace Ui {
class GGGeneratorDialog;
}

class GGGeneratorDialog : public QDialog, public GGGeneratorUIHost
{
    Q_OBJECT

public:
    explicit GGGeneratorDialog(GGEditProject *project, const QDir &pluginDir, QWidget *parent = 0);
    ~GGGeneratorDialog();

    virtual bool openExternalFileEditor(const QString &file);
    virtual void notifyGenerateEnabled(GGGeneratorInterface *generator, bool enabled);

public slots:
    void enableGeneration(bool enable);
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

    void checkGeneratorEnabled();

private:
    GGGeneratorInterface *currentGenerator();
    void cleanUpGenerators();
    void doCheckGeneratorEnabled(bool isGeneratorEnabled);

private:
    Ui::GGGeneratorDialog *ui;
    GGEditProject *m_project;
    QPushButton *m_btnGenerate;
    GGGeneratorManager *m_manager;
    GGGeneratorInterface *m_curGenerator;

    QMap<GGGeneratorInterface *, QWidget *> m_generatorUIs;
};

#endif // GGGENERATORDIALOG_H
