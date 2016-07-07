#ifndef GGUTILITIES_H
#define GGUTILITIES_H

#include <gg_core_global.h>

class GG_CORESHARED_EXPORT GGUtilities
{
private:
    GGUtilities();
    Q_DISABLE_COPY(GGUtilities)

public:
    static QString sanatizeFileName(QString name);
    static QString versionToString(quint32 version);
    static quint32 stringToVersion(QString str);
    static QString escapeHtml(QString text);
};

#endif // GGUTILITIES_H
