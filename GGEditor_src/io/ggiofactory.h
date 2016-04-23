#ifndef GGIOFACTORY_H
#define GGIOFACTORY_H

#include <QString>

class QIODevice;
class GGAbstractSerializationWriter;
class GGBasicProjectSerializer;
class GGViewProjectSerializer;

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

    static QString extensionForSerializationType(SerializationType type);
    static SerializationType serializationTypeForFile(const QString &fileName);

    static GGBasicProjectSerializer *basicSerializer(QIODevice *device, SerializationType type);
    static GGViewProjectSerializer *viewSerializer(QIODevice *device, SerializationType type);

private:
    static GGAbstractSerializationWriter *writerForType(SerializationType type, QIODevice *device);
};

#endif // GGIOFACTORY_H
