#ifndef GGBINARYUNSERIALIZATIONREADER_H
#define GGBINARYUNSERIALIZATIONREADER_H

#include <io/ggabstractunserializationreader.h>
#include <QString>

class GGBinaryUnserializationReader : public GGAbstractUnserializationReader
{
public:
    GGBinaryUnserializationReader();
    virtual ~GGBinaryUnserializationReader() {}

    virtual GGIOFactory::SerializationType serializationType() const;
    virtual bool unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder);
    virtual QString error() const;

protected:
    bool setError(const QString &err);

private:
    QString m_error;
};

#endif // GGBINARYUNSERIALIZATIONREADER_H
