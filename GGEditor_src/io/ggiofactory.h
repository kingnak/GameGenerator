#ifndef GGIOFACTORY_H
#define GGIOFACTORY_H

#include <QString>

class QIODevice;
class GGAbstractSerializationWriter;
class GGBasicProjectSerializer;
class GGViewProjectSerializer;
class GGAbstractProjectUnserializer;

class GGIOFactory
{
private:
    GGIOFactory();
    Q_DISABLE_COPY(GGIOFactory)

public:
    enum SerializationType {
        Unknown,
        BinaryModel,
        SimpleXMLModel
    };

    static const quint32 FILE_VERSION = 0x00010000U;
    static const char *BINARY_MODEL_HEADER;
    static const quint32 BINARY_MODEL_HEADER_SIZE = 4;

    static QString extensionForSerializationType(SerializationType type);
    static SerializationType serializationTypeForFile(const QString &fileName);

    static GGBasicProjectSerializer *basicSerializer(QIODevice *device, SerializationType type);
    static GGViewProjectSerializer *viewSerializer(QIODevice *device, SerializationType type);

    static GGAbstractProjectUnserializer *unserializer(const QString &file);
    static GGAbstractProjectUnserializer *unserializer(const QString &basePath, const QString &fileName, SerializationType type);

private:
    static GGAbstractSerializationWriter *writerForType(SerializationType type, QIODevice *device);
};

#endif // GGIOFACTORY_H
