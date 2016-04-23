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
    virtual bool writeProject(const QVariant &project);
    virtual bool writeProjectEnd();
    virtual bool writeSceneStart();
    virtual bool writeScene(const QVariant &scene);
    virtual bool writeSceneEnd();
    virtual bool writePagesStart();
    virtual bool writePage(const QVariant &page);
    virtual bool writePagesEnd();
    virtual bool writeConnectionsStart();
    virtual bool writeConnection(const QVariant &connection);
    virtual bool writeConnectionsEnd();
    virtual bool writeForeignPagesStart();
    virtual bool writeForeignPage(const QVariant &page);
    virtual bool writeForeignPagesEnd();
    virtual bool writeForeignConnectionsStart();
    virtual bool writeForeignConnection(const QVariant &connection);
    virtual bool writeForeignConnectionsEnd();

private:
    bool writeData(quint8 tag, const QVariant &data);

private:
    QIODevice *m_device;
    QDataStream *m_stream;
};

#endif // GGBINARYSERIALIZATIONWRITER_H
