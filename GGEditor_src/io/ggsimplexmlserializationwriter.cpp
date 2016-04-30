#include "ggsimplexmlserializationwriter.h"
#include <io/ggxmlserializer.h>
#include <io/ggiofactory.h>
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
    return true;
}

bool GGSimpleXmlSerializationWriter::writeFooter()
{
    m_stream->writeEndDocument();
    return true;
}

bool GGSimpleXmlSerializationWriter::writeProjectStart()
{
    m_stream->writeStartElement("GGProject");
    m_stream->writeTextElement("version", GGIOFactory::versionToString(GGIOFactory::FILE_VERSION));
    return true;
}

bool GGSimpleXmlSerializationWriter::writeProject(const QVariant &project)
{
    return m_writer->writeData(m_stream, "project", project);
}

bool GGSimpleXmlSerializationWriter::writeProjectEnd()
{
    m_stream->writeEndElement();
    return true;
}

bool GGSimpleXmlSerializationWriter::writeSceneStart()
{
    m_stream->writeStartElement("GGScene");
    return true;
}

bool GGSimpleXmlSerializationWriter::writeScene(const QVariant &scene)
{
    return m_writer->writeData(m_stream, "scene", scene, QSet<QString>() << "id");
}

bool GGSimpleXmlSerializationWriter::writeSceneEnd()
{
    m_stream->writeEndElement();
    return true;
}

bool GGSimpleXmlSerializationWriter::writePagesStart()
{
    return true;
}

bool GGSimpleXmlSerializationWriter::writePage(const QVariant &page)
{
    return m_writer->writeData(m_stream, "page", page, QSet<QString>() << "id");
}

bool GGSimpleXmlSerializationWriter::writePagesEnd()
{
    return true;
}

bool GGSimpleXmlSerializationWriter::writeConnectionsStart()
{
    return true;
}

bool GGSimpleXmlSerializationWriter::writeConnection(const QVariant &connection)
{
    return m_writer->writeData(m_stream, "connection", connection, QSet<QString>() << "id");
}

bool GGSimpleXmlSerializationWriter::writeConnectionsEnd()
{
    return true;
}

bool GGSimpleXmlSerializationWriter::writeForeignPagesStart()
{
    m_stream->writeStartElement("pageReferences");
    return true;
}

bool GGSimpleXmlSerializationWriter::writeForeignPage(const QVariant &page)
{
    return m_writer->writeData(m_stream, "pageRef", page, QSet<QString>() << "pageId");
}

bool GGSimpleXmlSerializationWriter::writeForeignPagesEnd()
{
    m_stream->writeEndElement();
    return true;
}

bool GGSimpleXmlSerializationWriter::writeForeignConnectionsStart()
{
    m_stream->writeStartElement("connectionReferences");
    return true;
}

bool GGSimpleXmlSerializationWriter::writeForeignConnection(const QVariant &connection)
{
    return m_writer->writeData(m_stream, "connectionRef", connection, QSet<QString>() << "connectionId");
}

bool GGSimpleXmlSerializationWriter::writeForeignConnectionsEnd()
{
    m_stream->writeEndElement();
    return true;
}
