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
