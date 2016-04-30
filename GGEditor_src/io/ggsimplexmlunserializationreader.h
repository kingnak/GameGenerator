#ifndef GGSIMPLEXMLUNSERIALIZATIONREADER_H
#define GGSIMPLEXMLUNSERIALIZATIONREADER_H

#include <io/ggabstractunserializationreader.h>
#include <io/ggxmlunserializer.h>

class GGSimpleXmlUnserializationReader : public GGAbstractUnserializationReader, public GGXmlUnserializerHandler
{
public:
    GGSimpleXmlUnserializationReader();
    ~GGSimpleXmlUnserializationReader();

    virtual GGIOFactory::SerializationType serializationType() const;
    virtual bool unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder);
    virtual QString error() const;

    virtual HandleType handleElement(QString &name, QVariant &data);

private:
    GGAbstractProjectUnserializer *m_builder;
    QString m_error;
};

#endif // GGSIMPLEXMLUNSERIALIZATIONREADER_H
