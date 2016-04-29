#include "ggsimplexmlunserializationreader.h"
#include <io/ggxmlunserializer.h>
#include <io/ggserialization.hpp>
#include <QRect>

GGSimpleXmlUnserializationReader::GGSimpleXmlUnserializationReader()
{

}

GGSimpleXmlUnserializationReader::~GGSimpleXmlUnserializationReader()
{

}

bool GGSimpleXmlUnserializationReader::unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder)
{
    GGXmlUnserializer ser(this);
    return ser.load(device);
}

GGXmlUnserializerHandler::HandleType GGSimpleXmlUnserializationReader::handleElement(QString &name, QVariant &data)
{
    QVariantMap m;
    data >> m;
    if (name == "rect" && m.size() == 4) {
        // Assume x,y,w,h
        bool ok;
        int x = m["x"].toInt(&ok);
        if (!ok) return Error;
        int y = m["y"].toInt(&ok);
        if (!ok) return Error;
        int w = m["w"].toInt(&ok);
        if (!ok) return Error;
        int h = m["h"].toInt(&ok);
        if (!ok) return Error;

        QRect r(x,y,w,h);
        data = r;
        return Push;
    }

    if (m.contains(" text ")) {
        if (m.size() != 1) {
            name = "Only text allowed as content";
            return Error;
        }
        data = m[" text "];
        return Push;
    }

    return Push;
}
