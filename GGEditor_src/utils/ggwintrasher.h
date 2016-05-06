#ifndef GGWINTRASHER_H
#define GGWINTRASHER_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include "ggtrasher.h"

class GGWinTrasher : public GGTrasher
{
public:
    GGWinTrasher();

    bool moveToTrash(const QString &file);
};
#endif

#endif // GGWINTRASHER_H
