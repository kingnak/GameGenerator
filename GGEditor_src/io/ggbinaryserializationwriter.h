#ifndef GGBINARYSERIALIZATIONWRITER_H
#define GGBINARYSERIALIZATIONWRITER_H

#include <io/ggabstractserializationwriter.h>

class QIODevice;
class QDataStream;

class GGBinarySerializationWriter : public GGAbstractSerializationWriter
{
public:
    GGBinarySerializationWriter(QIODevice *device);
    ~GGBinarySerializationWriter();

    virtual bool writeHeader();
    virtual bool writeFooter();

    virtual bool writeProjectStart();
    virtual bool writeProject(QVariant project);
    virtual bool writeProjectEnd();
    virtual bool writeSceneStart();
    virtual bool writeScene(QVariant scene);
    virtual bool writeSceneEnd();
    virtual bool writePagesStart();
    virtual bool writePage(QVariant page);
    virtual bool writePagesEnd();
    virtual bool writeConnectionsStart();
    virtual bool writeConnection(QVariant connection);
    virtual bool writeConnectionsEnd();

private:
    bool writeData(char tag, const QVariant &data);

private:
    QIODevice *m_device;
    QDataStream *m_stream;
};

#endif // GGBINARYSERIALIZATIONWRITER_H
