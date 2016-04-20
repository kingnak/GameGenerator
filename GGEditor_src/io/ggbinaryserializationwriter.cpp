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

bool GGBinarySerializationWriter::writeProject(QVariant project)
{
    return writeData('P', project);
}

bool GGBinarySerializationWriter::writeProjectEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeSceneStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeScene(QVariant scene)
{
    return writeData('s', scene);
}

bool GGBinarySerializationWriter::writeSceneEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writePagesStart()
{
    return true;
}

bool GGBinarySerializationWriter::writePage(QVariant page)
{
    return writeData('p', page);
}

bool GGBinarySerializationWriter::writePagesEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeConnectionsStart()
{
    return true;
}

bool GGBinarySerializationWriter::writeConnection(QVariant connection)
{
    return writeData('c', connection);
}

bool GGBinarySerializationWriter::writeConnectionsEnd()
{
    return true;
}

bool GGBinarySerializationWriter::writeData(char tag, const QVariant &data)
{
    (*m_stream) << tag;
    (*m_stream) << data;
    return true;
}
