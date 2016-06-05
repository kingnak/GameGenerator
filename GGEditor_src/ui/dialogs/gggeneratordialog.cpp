#include "gggeneratordialog.h"
#include "ui_gggeneratordialog.h"
#include <generator/gggeneratormanager.h>
#include <model/ggeditproject.h>
#include <QtWidgets>
#include <QDebug>

GGGeneratorDialog::GGGeneratorDialog(GGEditProject *project, const QDir &pluginDir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGGeneratorDialog),
    m_project(project),
    m_curGenerator(NULL)
{
    ui->setupUi(this);
    m_btnGenerate = new QPushButton("Generate", this);
    ui->buttonBox->addButton(m_btnGenerate, QDialogButtonBox::ActionRole);
    connect(m_btnGenerate, SIGNAL(clicked(bool)), this, SLOT(generate()));
    disableGeneration();

    showGenerator(NULL);
    m_manager = new GGGeneratorManager(this);
    loadGenerators(project->basePath(), pluginDir);
}

GGGeneratorDialog::~GGGeneratorDialog()
{
    delete ui;
}

void GGGeneratorDialog::enableGeneration(bool enable)
{
    m_btnGenerate->setEnabled(enable);
}

void GGGeneratorDialog::disableGeneration(bool disable)
{
    m_btnGenerate->setDisabled(disable);
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

            // TODO: Clean directory

            if (!currentGenerator()->generate(m_project->model(), out)) {
                QMessageBox::critical(this, "Generator", "Error generating game");
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

    ui->stkGenerators->addWidget(gen->ui());
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
    ui->stkGenerators->setCurrentWidget(generator->ui());
    ui->lblGeneratorDescription->setText(generator->description());
    ui->lblGeneratorName->setText(generator->name());
    m_curGenerator = generator;
}

bool GGGeneratorDialog::saveChanges()
{
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
        ui->txtOutput->setText(d);
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
