#include "ggsimplexmlserializationwriter.h"
#include <io/ggxmlserializer.h>
#include <QXmlStreamWriter>

GGSimpleXmlSerializationWriter::GGSimpleXmlSerializationWriter(QIODevice *device)
{
    m_writer = new GGXmlSerializer;
    m_stream = new QXmlStreamWriter(device);
    m_stream->setAutoFormatting(true);
}

GGSimpleXmlSerializationWriter::~GGSimpleXmlSerializationWriter()
{
    delete m_stream;
    delete m_writer;
}

bool GGSimpleXmlSerializationWriter::writeHeader()
{
    m_stream->writeStartDocument();
    m_stream->writeStartElement("GGProject");
    m_stream->writeAttribute("version", "0.1.0.0");
    return true;
}

bool GGSimpleXmlSerializationWriter::writeFooter()
{
    m_stream->writeEndDocument();
    return true;
}

bool GGSimpleXmlSerializationWriter::writeProject(QVariant project)
{
    return m_writer->writeData(m_stream, "project", project);
}

bool GGSimpleXmlSerializationWriter::writeScene(QVariant scene)
{
    return m_writer->writeData(m_stream, "scene", scene, QSet<QString>() << "id");
}

bool GGSimpleXmlSerializationWriter::writePage(QVariant page)
{
    return m_writer->writeData(m_stream, "page", page, QSet<QString>() << "id");
}

bool GGSimpleXmlSerializationWriter::writeConnection(QVariant connection)
{
    return m_writer->writeData(m_stream, "connection", connection, QSet<QString>() << "id");
}
