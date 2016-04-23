#ifndef GGABSTRACTUNSERIALIZATIONREADER_H
#define GGABSTRACTUNSERIALIZATIONREADER_H

class GGAbstractProjectUnserializer;
class QIODevice;

class GGAbstractUnserializationReader
{
public:
    virtual ~GGAbstractUnserializationReader() {}

    virtual bool unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder) = 0;
};

#endif // GGABSTRACTUNSERIALIZATIONREADER_H
