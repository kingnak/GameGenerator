#include "ggiofactory.h"
#include <io/ggbasicprojectserializer.h>
#include <io/ggviewprojectserializer.h>
#include <io/ggserializationprocessor.h>
#include <io/ggunserializationprocessor.h>
#include <io/ggbinaryserializationwriter.h>
#include <io/ggsimplexmlserializationwriter.h>
#include <io/ggbinaryunserializationreader.h>
#include <io/ggsimplexmlunserializationreader.h>
#include <io/ggviewprojectunserializer.h>

const char *GGIOFactory::BINARY_MODEL_HEADER = "GGMB";

GGIOFactory::GGIOFactory()
{

}

QString GGIOFactory::versionToString(quint32 version)
{
    quint8 M,m,p,b;
    b = version & 0xFF; version >>= 8;
    p = version & 0xFF; version >>= 8;
    m = version & 0xFF; version >>= 8;
    M = version & 0xFF; version >>= 8;
    return QString("%1.%2.%3.%4").arg(M).arg(m).arg(p).arg(b);
}

quint32 GGIOFactory::stringToVersion(QString str)
{
    QRegExp rx("(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)");
    if (rx.exactMatch(str)) {
        quint16 M,m,p,b;
        bool ok;
        M = rx.cap(1).toUShort(&ok); if (!ok || M > 255) return 0xFFFFFFFF;
        m = rx.cap(2).toUShort(&ok); if (!ok || m > 255) return 0xFFFFFFFF;
        p = rx.cap(3).toUShort(&ok); if (!ok || p > 255) return 0xFFFFFFFF;
        b = rx.cap(4).toUShort(&ok); if (!ok || b > 255) return 0xFFFFFFFF;

        quint32 v = 0;
        v <<= 8; v |= M;
        v <<= 8; v |= m;
        v <<= 8; v |= p;
        v <<= 8; v |= b;

        return v;
    }
    return 0xFFFFFFFF;
}

QString GGIOFactory::extensionForSerializationType(GGIOFactory::SerializationType type)
{
    switch (type) {
    case Unknown: return "";
    case SimpleXMLModel: return "ggmx";
    case BinaryModel: return "ggmb";
    default: return "";
    }
}

GGIOFactory::SerializationType GGIOFactory::serializationTypeForFile(const QString &fileName)
{
    QString ext = fileName.right(fileName.length() - fileName.lastIndexOf('.')).toLower();
    if (ext == ".ggmx") {
        return SimpleXMLModel;
    }
    if (ext == ".ggmb") {
        return BinaryModel;
    }
    return Unknown;
}

GGBasicProjectSerializer *GGIOFactory::basicSerializer(QIODevice *device, GGIOFactory::SerializationType type)
{
    GGAbstractSerializationWriter *writer = writerForType(type, device);
    return new GGBasicProjectSerializer(writer, new GGDefaultSerializationProcessor);
}

GGViewProjectSerializer *GGIOFactory::viewSerializer(QIODevice *device, GGIOFactory::SerializationType type)
{
    GGAbstractSerializationWriter *writer = writerForType(type, device);
    return new GGViewProjectSerializer(writer, new GGDefaultSerializationProcessor);
}

GGViewProjectUnserializer *GGIOFactory::unserializer(const QString &file)
{
    QFileInfo fi(file);
    return unserializer(fi.absolutePath(), fi.baseName(), serializationTypeForFile(file));
}

GGViewProjectUnserializer *GGIOFactory::unserializer(const QString &basePath, const QString &fileName, GGIOFactory::SerializationType type)
{
    GGAbstractUnserializationReader *reader;
    switch (type) {
    case SimpleXMLModel: reader = new GGSimpleXmlUnserializationReader; break;
    case BinaryModel: reader = new GGBinaryUnserializationReader; break;
    case Unknown: return NULL;
    }

    GGViewProjectUnserializer *ser = new GGViewProjectUnserializer(basePath, fileName, reader, new GGDefaultUnserializationProcessor);
    return ser;
}

GGAbstractSerializationWriter *GGIOFactory::writerForType(GGIOFactory::SerializationType type, QIODevice *device)
{
    switch (type) {
    case SimpleXMLModel: return new GGSimpleXmlSerializationWriter(device);
    case BinaryModel: return new GGBinarySerializationWriter(device);
    case Unknown:
    default:
        return NULL;
    }
}

