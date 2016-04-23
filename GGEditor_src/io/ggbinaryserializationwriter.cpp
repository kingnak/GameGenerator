#include "ggbinaryserializationwriter.h"
#include <QDataStream>

GGBinarySerializationWriter::GGBinarySerializationWriter(QIODevice *device)
    : m_device(device)
{
    m_stream = new QDataStream(m_device);
    m_stream->setVersion(QDataStream::Qt_5_5);
}

GGBinarySerializationWriter::~GGBinarySerializationWriter()
{
    delete m_stream;
}

bool GGBinarySerializationWriter::writeHeader()
{
    const char HEADER[] = "GGPB";
    const quint32 VERSION = 0x00010000; // V 0.1.0.0
    m_stream->writeRawData(HEADER, sizeof(HEADER)-1);
    (*m_stream) << VERSION;
    return true;
}

bool GGBinarySerializationWriter::writeFooter()
{
    return true;
}

bool GGBinarySerializationWriter::writeProjectStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeProject(const QVariant &project)
{
    return writeData('M', project);
}

bool GGBinarySerializationWriter::writeScene(const QVariant &scene)
{
    return writeData('S', scene);
}

bool GGBinarySerializationWriter::writePage(const QVariant &page)
{
    return writeData('P', page);
}

bool GGBinarySerializationWriter::writeConnection(const QVariant &connection)
{
    return writeData('C', connection);
}

bool GGBinarySerializationWriter::writeForeignPage(const QVariant &page)
{
    return writeData('p', page);
}

bool GGBinarySerializationWriter::writeForeignConnection(const QVariant &connection)
{
    return writeData('c', connection);
}

bool GGBinarySerializationWriter::writeProjectEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeSceneStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeSceneEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writePagesStart()
{
    return true;
}

bool GGBinarySerializationWriter::writePagesEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeConnectionsStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeConnectionsEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeForeignPagesStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeForeignPagesEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeForeignConnectionsStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeForeignConnectionsEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeData(char tag, const QVariant &data)
{
    (*m_stream) << tag;
    (*m_stream) << data;
    return true;
}
