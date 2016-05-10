#include "ggsimplexmlunserializationreader.h"
#include <io/ggxmlunserializer.h>
#include <io/ggserialization.hpp>
#include <io/ggabstractprojectunserializer.h>
#include <QRect>
#include <QColor>

GGSimpleXmlUnserializationReader::GGSimpleXmlUnserializationReader()
    : m_builder(NULL)
{
}

GGSimpleXmlUnserializationReader::~GGSimpleXmlUnserializationReader()
{

}

GGIOFactory::SerializationType GGSimpleXmlUnserializationReader::serializationType() const
{
    return GGIOFactory::SimpleXMLModel;
}

bool GGSimpleXmlUnserializationReader::unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder)
{
    GGXmlUnserializer ser(this);
    m_builder = projectBuilder;
    bool ret = ser.load(device);
    m_error = ser.errorString();
    return ret;
}

QString GGSimpleXmlUnserializationReader::error() const
{
    return m_error;
}

GGXmlUnserializerHandler::HandleType GGSimpleXmlUnserializationReader::handleElement(QString &name, QVariant &data)
{
    QVariantMap m;
    data >> m;

    if (name == "version") {
        quint32 v = GGIOFactory::stringToVersion(m[" text "].toString());
        if (v != GGIOFactory::FILE_VERSION) {
            name = "Unsupported version";
            return Error;
        }
        return Pop;
    }

    // Fix Rects
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

    // Fix Colors
    if (m.size() == 1 && m.contains("rgb")) {
        bool ok;
        quint32 v = m["rgb"].toString().toUInt(&ok, 16);
        if (!ok) return Error;
        v &= 0xFFFFFF;
        QColor c = QColor::fromRgb((QRgb) v);
        data = QVariant::fromValue(c);
        return Push;
    }

    // Set Text as only content
    if (m.contains(" text ")) {
        if (m.size() != 1) {
            name = "Only text allowed as content";
            return Error;
        }
        data = m[" text "];
        return Push;
    }

    // Handle Lists
    HandleType t = handleLists(name, data, m);
    if (t != Pop) return t;

    if (name == "project") {
        bool ok = m_builder->unserializeProject(data);
        if (!ok) return Error;
        return Pop;
    }
    if (name == "scene") {
        bool ok = m_builder->unserializeScene(data);
        if (!ok) return Error;
        return Pop;
    }
    if (name == "page") {
        bool ok = m_builder->unserializePage(data);
        if (!ok) return Error;
        return Pop;
    }
    if (name == "connection") {
        bool ok = m_builder->unserializeConnection(data);
        if (!ok) return Error;
        return Pop;
    }
    if (name == "pageRef") {
        bool ok = m_builder->unserializeForeignPage(data);
        if (!ok) return Error;
        return Pop;
    }
    if (name == "connectionRef") {
        bool ok = m_builder->unserializeForeignConnection(data);
        if (!ok) return Error;
        return Pop;
    }

    // Handle Markers
    if (name == "pageReferences") {
        Q_ASSERT(m.isEmpty());
        return Pop;
    }
    if (name == "connectionReferences") {
        Q_ASSERT(m.isEmpty());
        return Pop;
    }
    if (name == "GGScene") {
        Q_ASSERT(m.isEmpty());
        return Pop;
    }
    if (name == "GGProject") {
        Q_ASSERT(m.isEmpty());
        return Pop;
    }

    if (name == " ROOT ") {
        Q_ASSERT(m.isEmpty());
        bool ok = m_builder->finalizeUnserialization();
        if (!ok) {
            name = "Reference Error";
            return Error;
        }
        return Pop;
    }

    return Push;
}

GGXmlUnserializerHandler::HandleType GGSimpleXmlUnserializationReader::handleLists(QString &name, QVariant &data, const QVariantMap &map)
{
    HandleType t = doHandleList("variable", name, data, map);
    if (t != Pop) return t;
    t = doHandleList("viewPage", name, data, map);
    if (t != Pop) return t;
    t = doHandleList("viewConnection", name, data, map);
    if (t != Pop) return t;
    t = doHandleList("media", name, data, map);
    if (t != Pop) return t;
    t = doHandleList("map", name, data, map);
    if (t != Pop) return t;
    t = doHandleList("decision", name, data, map);
    if (t != Pop) return t;
    t = doHandleList("style", name, data, map);
    if (t != Pop) return t;

    return Pop;
}

GGXmlUnserializerHandler::HandleType GGSimpleXmlUnserializationReader::doHandleList(const QString &itemName, QString &name, QVariant &data, const QVariantMap &map)
{
    if (name == itemName) {
        return PushList;
    }
    if (name == itemName + "s") {
        name = itemName;
        data = map[itemName];
        return Push;
    }
    return Pop;
}
