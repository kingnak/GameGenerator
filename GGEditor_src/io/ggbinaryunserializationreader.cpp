#include "ggbinaryunserializationreader.h"
#include <io/ggiofactory.h>
#include <io/ggabstractprojectunserializer.h>
#include <QIODevice>
#include <QDataStream>
#include <QVariantMap>

GGBinaryUnserializationReader::GGBinaryUnserializationReader()
{

}

bool GGBinaryUnserializationReader::unserialize(QIODevice *device, GGAbstractProjectUnserializer *projectBuilder)
{
    QDataStream stream(device);
    stream.setVersion(QDataStream::Qt_5_5);

    { // HEADER
        char buf[GGIOFactory::BINARY_MODEL_HEADER_SIZE];
        stream.readRawData(buf, GGIOFactory::BINARY_MODEL_HEADER_SIZE);
        QString str = QString::fromUtf8(buf, GGIOFactory::BINARY_MODEL_HEADER_SIZE);
        if (str != QString::fromUtf8(GGIOFactory::BINARY_MODEL_HEADER)) {
            return false;
        }

        quint32 version;
        stream >> version;
        if (version != GGIOFactory::FILE_VERSION) {
            return false;
        }
    }

    bool ok = true;
    while (ok && !stream.atEnd() && stream.status() == QDataStream::Ok) {
        quint8 t;
        QVariant v;
        stream >> t;
        stream >> v;

        switch (t) {
        case 'M':
            ok &= projectBuilder->unserializeProject(v);
            break;
        case 'S':
            ok &= projectBuilder->unserializeScene(v);
            break;
        case 'P':
            ok &= projectBuilder->unserializePage(v);
            break;
        case 'C':
            ok &= projectBuilder->unserializeConnection(v);
            break;
        default:
            ok = false;
        }
    }

    ok &= stream.status() == QDataStream::Ok && stream.atEnd();
    if (ok) {
        ok &= projectBuilder->finalizeUnserialization();
    }
    return ok;
}
