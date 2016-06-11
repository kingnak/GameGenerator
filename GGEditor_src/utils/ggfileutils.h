#ifndef GGFILEUTILS_H
#define GGFILEUTILS_H

#include <QDir>

class GGFileUtils
{
private:
    GGFileUtils() {}
    Q_DISABLE_COPY(GGFileUtils)

public:
    static bool isDirEmpty(const QDir &path);
    static bool recursiveClearDir(const QDir &path);

};

#endif // GGFILEUTILS_H
