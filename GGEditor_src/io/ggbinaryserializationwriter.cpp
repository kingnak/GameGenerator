#include "ggbinaryserializationwriter.h"
#include <io/ggiofactory.h>
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
    m_stream->writeRawData(GGIOFactory::BINARY_MODEL_HEADER, GGIOFactory::BINARY_MODEL_HEADER_SIZE);
    (*m_stream) << GGIOFactory::FILE_VERSION;
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

bool GGBinarySerializationWriter::writeData(quint8 tag, const QVariant &data)
{
    (*m_stream) << tag;
    (*m_stream) << data;
    return true;
}
