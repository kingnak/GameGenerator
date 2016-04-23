#ifndef GGSIMPLEXMLSERIALIZATIONWRITER_H
#define GGSIMPLEXMLSERIALIZATIONWRITER_H

#include <io/ggabstractserializationwriter.h>

class QIODevice;
class QXmlStreamWriter;
class GGXmlSerializer;

class GGSimpleXmlSerializationWriter : public GGAbstractSerializationWriter
{
public:
    GGSimpleXmlSerializationWriter(QIODevice *device);
    virtual ~GGSimpleXmlSerializationWriter();

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
    GGXmlSerializer *m_writer;
    QXmlStreamWriter *m_stream;
};

#endif // GGSIMPLEXMLSERIALIZATIONWRITER_H
