#ifndef GGABSTRACTUNSERIALIZATIONREADER_H
#define GGABSTRACTUNSERIALIZATIONREADER_H

#include <io/ggiofactory.h>

class GGAbstractProjectUnserializer;
class QIODevice;
class QString;

class GGAbstractUnserializationReader
{
public:
    virtual ~GGAbstractUnserializationReader() {}

    virtual GGIOFactory::SerializationType serializationType() const = 0;
    virtual bool unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder) = 0;
    virtual QString error() const = 0;
};

#endif // GGABSTRACTUNSERIALIZATIONREADER_H
