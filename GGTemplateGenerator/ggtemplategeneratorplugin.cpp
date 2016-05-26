#include "ggtemplategeneratorplugin.h"


GGTemplateGeneratorPlugin::GGTemplateGeneratorPlugin(QObject *parent) :
    QObject(parent)
{
}

GGTemplateGeneratorPlugin::~GGTemplateGeneratorPlugin()
{

}

void GGTemplateGeneratorPlugin::setBaseDir(const QDir &dir)
{
    m_baseDir = dir;
}

bool GGTemplateGeneratorPlugin::activate()
{
    return true;
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

QWidget *GGTemplateGeneratorPlugin::ui()
{
    return NULL;
}

bool GGTemplateGeneratorPlugin::generate(const GGAbstractModel *model, const QDir &output)
{
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GGTemplateGenerator, GGTemplateGeneratorPlugin)
#endif // QT_VERSION < 0x050000
