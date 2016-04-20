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

    virtual bool writeProject(QVariant project);
    virtual bool writeScene(QVariant scene);
    virtual bool writePage(QVariant page);
    virtual bool writeConnection(QVariant connection);

private:
    GGXmlSerializer *m_writer;
    QXmlStreamWriter *m_stream;
};

#endif // GGSIMPLEXMLSERIALIZATIONWRITER_H
