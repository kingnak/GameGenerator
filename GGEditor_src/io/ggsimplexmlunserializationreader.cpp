#include "ggsimplexmlunserializationreader.h"
#include <ggutilities.h>
#include <io/ggxmlunserializer.h>
#include <io/ggserialization.hpp>
#include <io/ggabstractprojectunserializer.h>
#include <QRect>
#include <QColor>

GGSimpleXmlUnserializationReader::GGSimpleXmlUnserializationReader()
    : m_builder(NULL)
{
    addListType("varDefinition");
    addListType("viewPage");
    addListType("viewConnection");
    addListType("media");
    addListType("map");
    addListType("decision");
    addListType("style");
    addListType("openScene");
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
        quint32 v = GGUtilities::stringToVersion(m[" text "].toString());
        if (v != GGIOFactory::FILE_VERSION) {
            name = "Unsupported version";
            return Error;
        }
        return Pop;
    }

    // Handle defaults
    HandleType t = handleDefaults(m, name, data);
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
