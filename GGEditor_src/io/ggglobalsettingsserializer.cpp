#include "ggglobalsettingsserializer.h"
#include <io/ggiofactory.h>
#include <utils/ggglobaluserinfo.h>
#include <ggutilities.h>
#include <io/ggxmlserializer.h>
#include <io/ggxmlunserializer.h>
#include <QXmlStreamWriter>
#include <QDir>
#include <QStandardPaths>

GGGlobalSettingsSerializer::GGGlobalSettingsSerializer()
{

}

QString GGGlobalSettingsSerializer::defaultSettingsFilePath()
{
    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    dataDir.mkpath(".");
    return dataDir.absoluteFilePath("userData.xml");
}

bool GGGlobalSettingsSerializer::saveDefaultFile(const GGGlobalUserInfo *info)
{
    QFile f(GGGlobalSettingsSerializer::defaultSettingsFilePath());
    if (f.open(QIODevice::WriteOnly)) {
        GGGlobalSettingsSerializer ser;
        return ser.save(&f, info);
    }
    return false;
}

bool GGGlobalSettingsSerializer::loadDefaultFile(GGGlobalUserInfo *info)
{
    QFile f(GGGlobalSettingsSerializer::defaultSettingsFilePath());
    if (f.open(QIODevice::ReadOnly)) {
        GGGlobalSettingsSerializer ser;
        return ser.load(&f, info);
    }
    return false;
}

bool GGGlobalSettingsSerializer::save(QIODevice *dev, const GGGlobalUserInfo *info)
{
    GGXmlSerializer ser;
    QXmlStreamWriter str(dev);
    str.setAutoFormatting(true);
    str.writeStartDocument();
    str.writeStartElement("GGSettings");
    str.writeTextElement("version", GGUtilities::versionToString(GGIOFactory::FILE_VERSION));
    QVariant data;
    data << *info;
    bool ret = ser.writeData(&str, "UI", data);
    str.writeEndDocument();
    return ret;
}

bool GGGlobalSettingsSerializer::load(QIODevice *dev, GGGlobalUserInfo *info)
{
    GGDefaultXmlUnserializationHandler handler;
    handler.addListType("window");
    handler.addListType("splitter");
    GGXmlUnserializer ser(&handler);
    bool ret = ser.load(dev);
    if (!ret) return false;

    QVariantMap m = handler.getDocument();
    if (!m.contains("GGSettings")) {
        qDebug("Global settings has no GGSettings element");
        return false;
    }

    m = m["GGSettings"].value<QVariantMap>();
    if (m.contains("version")) {
        quint32 v = GGUtilities::stringToVersion(m["version"].toString());
        if (v != GGIOFactory::FILE_VERSION) {
            qDebug("Unsupported global settings version");
            return false;
        }
    }
    if (!m.contains("UI")) {
        qDebug("Global settings has no UI element");
        return false;
    }
    m["UI"].value<QVariantMap>() >> *info;
    return true;
}
