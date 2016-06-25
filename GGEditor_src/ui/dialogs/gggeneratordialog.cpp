#include "gggeneratordialog.h"
#include "ui_gggeneratordialog.h"
#include <generator/gggeneratormanager.h>
#include <model/ggeditproject.h>
#include <utils/ggglobaluserinfo.h>
#include <io/ggglobalsettingsserializer.h>
#include <utils/ggfileutils.h>
#include <model/ggmodelverifier.h>
#include <QtWidgets>
#include <QDebug>
#include <QProcess>

GGGeneratorDialog::GGGeneratorDialog(GGEditProject *project, const QDir &pluginDir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGGeneratorDialog),
    m_project(project),
    m_curGenerator(NULL)
{
    ui->setupUi(this);
    ui->txtOutput->setText(QDir::toNativeSeparators(GGGlobalUserInfo::instance().generatorOutputPath()));
    m_btnGenerate = new QPushButton("Generate", this);
    ui->buttonBox->addButton(m_btnGenerate, QDialogButtonBox::ActionRole);
    connect(m_btnGenerate, SIGNAL(clicked(bool)), this, SLOT(generate()));
    enableGeneration(false);

    showGenerator(NULL);
    m_manager = new GGGeneratorManager(this);
    loadGenerators(project->basePath(), pluginDir);
}

GGGeneratorDialog::~GGGeneratorDialog()
{
    delete ui;
}

bool GGGeneratorDialog::openExternalFileEditor(const QString &file)
{
    QString editor = GGGlobalUserInfo::instance().externalEditor();
    if (!QFile::exists(editor)) {
        return false;
    }
    return QProcess::startDetached(editor, QStringList()<<file);
}

void GGGeneratorDialog::notifyGenerateEnabled(GGGeneratorInterface *generator, bool enabled)
{
    if (generator != m_curGenerator || !m_curGenerator) {
        return;
    }

    doCheckGeneratorEnabled(enabled);
}

void GGGeneratorDialog::enableGeneration(bool enable)
{
    m_btnGenerate->setEnabled(enable);
}

void GGGeneratorDialog::accept()
{
    if (saveChanges()) {
        cleanUpGenerators();
        QDialog::accept();
    }
}

void GGGeneratorDialog::reject()
{
    cleanUpGenerators();
    QDialog::reject();
}

void GGGeneratorDialog::closeEvent(QCloseEvent *event)
{
    cleanUpGenerators();
    QDialog::closeEvent(event);
}

void GGGeneratorDialog::generate()
{
    if (currentGenerator()) {
        if (saveChanges()) {
            QDir out(ui->txtOutput->text());
            if (!out.mkpath(".")) {
                QMessageBox::critical(this, "Generator", "Cannot create output directoy");
                return;
            }

            // Clean directory
            if (!GGFileUtils::isDirEmpty(out)) {
                int res = QMessageBox::question(this, "Clear directory",
                                      QString("The directory\n%1\nis not empty.\n\nShould it be cleared? (All files will be deleted!)").arg(QDir::toNativeSeparators(out.absolutePath())),
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);

                if (res == QMessageBox::Yes) {
                    if (!GGFileUtils::recursiveClearDir(out)) {
                        QMessageBox::critical(this, "Generator", "Error clearing directory\n" + QDir::toNativeSeparators(out.absolutePath()));
                        return;
                    }
                } else if (res == QMessageBox::Cancel) {
                    return;
                }
            }

            // Verify model
            GGModelVerifier v;
            GGModelErrorList errs = v.verify(m_project->model());
            if (!errs.isEmpty()) {
                int res = QMessageBox::warning(this, "Generator", "There are errors in the current model. The output might not be working.", QMessageBox::Ok, QMessageBox::Cancel);
                if (res == QMessageBox::Cancel) {
                    return;
                }
            }

            qApp->setOverrideCursor(Qt::WaitCursor);
            bool ok = currentGenerator()->generate(m_project, out);
            qApp->restoreOverrideCursor();

            if (!ok) {
                QMessageBox::critical(this, "Generator", "Error generating game");
            } else {
                QMessageBox::information(this, "Generator", "Generation finished");
            }
        }
    }
}

void GGGeneratorDialog::loadGenerators(const QDir &generatorsDir, const QDir &pluginDir)
{
    if (!m_manager->loadGenerators(pluginDir)) {
        QMessageBox::critical(this, "Generator", "Cannot load generators");
        return;
    }

    QList<GGGeneratorInterface*> gens = m_manager->getGenerators();
    if (gens.isEmpty()) {
        QMessageBox::critical(this, "Generator", "No generators available");
        return;
    }

    if (gens.size() > 1) {
        struct GenComp {
            bool operator()(GGGeneratorInterface *g1, GGGeneratorInterface *g2) {
                return g1->name() < g2->name();
            }
        };

        qSort(gens.begin(), gens.end(), GenComp());
        qWarning() << "More than 1 generator. Only loading first";
    }

    GGGeneratorInterface *gen = gens.first();

    QDir genDir = generatorsDir;
    genDir.mkpath("generators");
    genDir.cd("generators");
    genDir.mkdir(gen->name());
    genDir.cd(gen->name());
    gen->setBaseDir(genDir);

    if (!gen->activate()) {
        QMessageBox::critical(this, "Generator", "Cannot activate Generator " + gen->name());
        return;
    }

    gen->loadSettings();

    QWidget *gui = gen->ui(this);
    m_generatorUIs[gen] = gui;
    ui->stkGenerators->addWidget(gui);
    showGenerator(gen);
}

void GGGeneratorDialog::showGenerator(GGGeneratorInterface *generator)
{
    if (!generator) {
        ui->lblGeneratorDescription->setText("");
        ui->lblGeneratorName->setText("");
        ui->stkGenerators->setCurrentWidget(ui->pageEmpty);
        m_curGenerator = NULL;
        return;
    }

    generator->loadSettings();
    ui->stkGenerators->setCurrentWidget(m_generatorUIs[generator]);
    ui->lblGeneratorDescription->setText(generator->description());
    ui->lblGeneratorName->setText(generator->name());
    m_curGenerator = generator;
    checkGeneratorEnabled();
}

bool GGGeneratorDialog::saveChanges()
{
    GGGlobalUserInfo::instance().setGeneratorOutputPath(QDir::toNativeSeparators(ui->txtOutput->text()));
    GGGlobalSettingsSerializer::saveDefaultFile(&GGGlobalUserInfo::instance());
    if (currentGenerator()) {
        if (!currentGenerator()->saveSettings()) {
            QMessageBox::critical(this, "Generator", "Cannot save generator settings");
            return false;
        }
    }
    return true;
}

void GGGeneratorDialog::on_btnBrowse_clicked()
{
    QString d = QFileDialog::getExistingDirectory(this, "Generator output", ui->txtOutput->text());
    if (!d.isNull()) {
        ui->txtOutput->setText(QDir::toNativeSeparators(d));
    }
    checkGeneratorEnabled();
}

void GGGeneratorDialog::checkGeneratorEnabled()
{
    if (m_curGenerator) {
        doCheckGeneratorEnabled(m_curGenerator->isGenerateEnabled());
    } else {
        doCheckGeneratorEnabled(false);
    }
}

GGGeneratorInterface *GGGeneratorDialog::currentGenerator()
{
    return m_curGenerator;
}

void GGGeneratorDialog::cleanUpGenerators()
{
    while (ui->stkGenerators->count() > 0) {
        QWidget *w = ui->stkGenerators->widget(0);
        ui->stkGenerators->removeWidget(w);
        w->setParent(NULL);
    }
    ui->stkGenerators->addWidget(ui->pageEmpty);
}

void GGGeneratorDialog::doCheckGeneratorEnabled(bool isGeneratorEnabled)
{
    if (!isGeneratorEnabled) {
        enableGeneration(false);
        return;
    }

    if (ui->txtOutput->text().isEmpty()) {
        enableGeneration(false);
    } else {
        enableGeneration(true);
    }
}
