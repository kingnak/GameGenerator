#include "ggtemplategeneratorplugin.h"
#include "settingsui.h"

GGTemplateGeneratorPlugin::GGTemplateGeneratorPlugin(QObject *parent) :
    QObject(parent)
{
    m_ui = new SettingsUI;
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
    QString fn = m_baseDir.absoluteFilePath("decision.html");
    if (!QFile::exists(fn)) {
        ok &= writeFile(fn, ":/templates/templates/decision.html");
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
    return true;
}

bool GGTemplateGeneratorPlugin::loadSettings()
{
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

QWidget *GGTemplateGeneratorPlugin::ui()
{
    return m_ui;
}

bool GGTemplateGeneratorPlugin::generate(const GGAbstractModel *model, const QDir &output)
{
    return true;
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
    quint64 s = out.write(ba);
    return s == ba.size();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GGTemplateGenerator, GGTemplateGeneratorPlugin)
#endif // QT_VERSION < 0x050000
