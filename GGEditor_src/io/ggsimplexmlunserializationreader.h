#ifndef GGSIMPLEXMLUNSERIALIZATIONREADER_H
#define GGSIMPLEXMLUNSERIALIZATIONREADER_H

#include <io/ggabstractunserializationreader.h>
#include <io/ggxmlunserializer.h>

class GGSimpleXmlUnserializationReader : public GGAbstractUnserializationReader, public GGXmlUnserializerHandler
{
public:
    GGSimpleXmlUnserializationReader();
    ~GGSimpleXmlUnserializationReader();

    virtual bool unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder);

    virtual HandleType handleElement(QString &name, QVariant &data);
};

#endif // GGSIMPLEXMLUNSERIALIZATIONREADER_H
