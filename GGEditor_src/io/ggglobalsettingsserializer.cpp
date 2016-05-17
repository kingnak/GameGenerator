#include "ggglobalsettingsserializer.h"
#include <io/ggiofactory.h>
#include <utils/ggglobaluserinfo.h>
#include <ggutilities.h>
#include <io/ggxmlserializer.h>
#include <io/ggxmlunserializer.h>
#include <QXmlStreamWriter>

GGGlobalSettingsSerializer::GGGlobalSettingsSerializer()
{

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
    handler.setRootElement("GGSettings");
    GGXmlUnserializer ser(&handler);
    bool ret = ser.load(dev);
    if (!ret) return false;
    QVariantMap m = handler.getDocument();
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
