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

};

#endif // GGUTILITIES_H
