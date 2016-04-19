#include "ggbinaryserializationwriter.h"
#include <QDataStream>

GGBinarySerializationWriter::GGBinarySerializationWriter(QIODevice *device)
    : m_device(device)
{
    m_stream = new QDataStream(m_device);
    m_stream->setVersion(QDataStream::Qt_5_5);
}

bool GGBinarySerializationWriter::writeProject(QVariant project)
{
    return writeData('P', project);
}

bool GGBinarySerializationWriter::writeScene(QVariant scene)
{
    return writeData('s', scene);
}

bool GGBinarySerializationWriter::writePage(QVariant page)
{
    return writeData('p', page);
}

bool GGBinarySerializationWriter::writeConnection(QVariant connection)
{
    return writeData('c', connection);
}

bool GGBinarySerializationWriter::writeData(char tag, const QVariant &data)
{
    (*m_stream) << tag;
    (*m_stream) << data;
    return true;
}
