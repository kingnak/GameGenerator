#include "ggtemplategeneratorplugin.h"
#include "settingsui.h"
#include "ui_settingsui.h"
#include <QSettings>
#include "ggtemplategenerator.h"

GGTemplateGeneratorPlugin::GGTemplateGeneratorPlugin(QObject *parent) :
    QObject(parent),
    m_host(NULL)
{
    m_ui = new SettingsUI;
    connect(m_ui, SIGNAL(openFileEditor(QString)), this, SLOT(editFile(QString)));
}

GGTemplateGeneratorPlugin::~GGTemplateGeneratorPlugin()
{
    delete m_ui;
}

void GGTemplateGeneratorPlugin::setBaseDir(const QDir &dir)
{
    m_baseDir = dir;
}

bool GGTemplateGeneratorPlugin::activate()
{
    bool ok = true;
    QString fn = m_baseDir.absoluteFilePath("condition.html");
    if (!QFile::exists(fn)) {
        ok &= writeFile(fn, ":/templates/templates/condition.html");
    }

    fn = m_baseDir.absoluteFilePath("page.html");
    if (!QFile::exists(fn)) {
        ok &= writeFile(fn, ":/templates/templates/page.html");
    }

    fn = m_baseDir.absoluteFilePath("functions.js");
    if (!QFile::exists(fn)) {
        ok &= writeFile(fn, ":/templates/templates/functions.js");
    }

    fn = m_baseDir.absoluteFilePath("styles.css");
    if (!QFile::exists(fn)) {
        ok &= writeFile(fn, ":/templates/templates/styles.css");
    }
    return ok;
}

bool GGTemplateGeneratorPlugin::deactivate()
{
    return true;
}

bool GGTemplateGeneratorPlugin::saveSettings()
{
    QString fn = getIniFileName();
    QSettings set(fn, QSettings::IniFormat);

    set.setValue("decisionsPerRow", m_ui->ui->spnDecisionsPerRow->value());
    set.sync();

    return true;
}

bool GGTemplateGeneratorPlugin::loadSettings()
{
    QString fn = getIniFileName();
    QSettings set(fn, QSettings::IniFormat);
    bool ok;
    int dpr = set.value("decisionsPerRow", 3).toInt(&ok);
    if (!ok) dpr = 3;

    m_ui->ui->spnDecisionsPerRow->setValue(dpr);
    m_ui->ui->txtPageTmpl->setText(QDir::toNativeSeparators(m_baseDir.absoluteFilePath("page.html")));
    m_ui->ui->txtConditionTmpl->setText(QDir::toNativeSeparators(m_baseDir.absoluteFilePath("condition.html")));
    m_ui->ui->txtCSSTmpl->setText(QDir::toNativeSeparators(m_baseDir.absoluteFilePath("styles.css")));
    m_ui->ui->txtJSTmpl->setText(QDir::toNativeSeparators(m_baseDir.absoluteFilePath("functions.js")));

    return true;
}

QString GGTemplateGeneratorPlugin::name() const
{
    return "HTML Template";
}

QString GGTemplateGeneratorPlugin::description() const
{
    return "A template based generator for HTML games";
}

QWidget *GGTemplateGeneratorPlugin::ui(GGGeneratorUIHost *host)
{
    m_host = host;
    return m_ui;
}

bool GGTemplateGeneratorPlugin::isGenerateEnabled()
{
    return true;
}

bool GGTemplateGeneratorPlugin::generate(const GGProject *project, const QDir &output)
{
    GGTemplateGenerator gen;
    gen.setConditionFile(m_ui->ui->txtConditionTmpl->text())
            .setCSSFile(m_ui->ui->txtCSSTmpl->text())
            .setDicisionCount(m_ui->ui->spnDecisionsPerRow->value())
            .setJSFile(m_ui->ui->txtJSTmpl->text())
            .setPageFile(m_ui->ui->txtPageTmpl->text())
            .setOutputDirectory(output.absolutePath());

    if (!gen.generate(project)) {
        QString err = gen.error();
        return false;
    }
    return true;
}

void GGTemplateGeneratorPlugin::editFile(const QString &file)
{
    if (m_host) {
        m_host->openExternalFileEditor(file);
    }
}

bool GGTemplateGeneratorPlugin::writeFile(QString dest, QString src)
{
    QFile out(dest);
    QFile in(src);
    if (!in.open(QIODevice::ReadOnly)) {
        return false;
    }

    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QByteArray ba = in.readAll();
    qint64 s = out.write(ba);
    return s == ba.size();
}

QString GGTemplateGeneratorPlugin::getIniFileName() const
{
    return m_baseDir.absoluteFilePath("settings.ini");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GGTemplateGenerator, GGTemplateGeneratorPlugin)
#endif // QT_VERSION < 0x050000
