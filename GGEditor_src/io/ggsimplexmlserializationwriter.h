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
    GGXmlSerializer *m_writer;
    QXmlStreamWriter *m_stream;
};

#endif // GGSIMPLEXMLSERIALIZATIONWRITER_H
