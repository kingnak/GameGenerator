#ifndef GGBINARYSERIALIZATIONWRITER_H
#define GGBINARYSERIALIZATIONWRITER_H

#include <io/ggabstractserializationwriter.h>

class QIODevice;
class QDataStream;

class GGBinarySerializationWriter : public GGAbstractSerializationWriter
{
public:
    GGBinarySerializationWriter(QIODevice *device);

    virtual bool writeProject(QVariant project);
    virtual bool writeScene(QVariant scene);
    virtual bool writePage(QVariant page);
    virtual bool writeConnection(QVariant connection);

private:
    bool writeData(char tag, const QVariant &data);

private:
    QIODevice *m_device;
    QDataStream *m_stream;
};

#endif // GGBINARYSERIALIZATIONWRITER_H
