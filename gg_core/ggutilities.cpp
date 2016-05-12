#include "ggutilities.h"
#include <QString>
#include <QRegExp>

QString GGUtilities::sanatizeFileName(QString name)
{
    name = QString::fromLocal8Bit(name.toLocal8Bit());
    name = name.replace(QRegExp("\\s+"), " ");
    name = name.trimmed();
    name = name.toLower();
    name = name.replace(QRegExp("[\\\\/\\|$:\" ]"), "_");
    return name;
}

QString GGUtilities::versionToString(quint32 version)
{
    quint8 M,m,p,b;
    b = version & 0xFF; version >>= 8;
    p = version & 0xFF; version >>= 8;
    m = version & 0xFF; version >>= 8;
    M = version & 0xFF; version >>= 8;
    return QString("%1.%2.%3.%4").arg(M).arg(m).arg(p).arg(b);
}

quint32 GGUtilities::stringToVersion(QString str)
{
    QRegExp rx("(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)");
    if (rx.exactMatch(str)) {
        quint16 M,m,p,b;
        bool ok;
        M = rx.cap(1).toUShort(&ok); if (!ok || M > 255) return 0xFFFFFFFF;
        m = rx.cap(2).toUShort(&ok); if (!ok || m > 255) return 0xFFFFFFFF;
        p = rx.cap(3).toUShort(&ok); if (!ok || p > 255) return 0xFFFFFFFF;
        b = rx.cap(4).toUShort(&ok); if (!ok || b > 255) return 0xFFFFFFFF;

        quint32 v = 0;
        v <<= 8; v |= M;
        v <<= 8; v |= m;
        v <<= 8; v |= p;
        v <<= 8; v |= b;

        return v;
    }
    return 0xFFFFFFFF;
}
