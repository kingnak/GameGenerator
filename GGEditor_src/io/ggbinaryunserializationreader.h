#ifndef GGBINARYUNSERIALIZATIONREADER_H
#define GGBINARYUNSERIALIZATIONREADER_H

#include <io/ggabstractunserializationreader.h>

class GGBinaryUnserializationReader : public GGAbstractUnserializationReader
{
public:
    GGBinaryUnserializationReader();
    virtual ~GGBinaryUnserializationReader() {}

    virtual bool unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder);
};

#endif // GGBINARYUNSERIALIZATIONREADER_H
