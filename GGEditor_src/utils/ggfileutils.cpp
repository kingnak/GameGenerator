#include "ggfileutils.h"

bool GGFileUtils::isDirEmpty(const QDir &path)
{
    QStringList l = path.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    return l.isEmpty();
}

bool GGFileUtils::recursiveClearDir(const QDir &path)
{
    bool ok = true;
    QFileInfoList files = path.entryInfoList(QDir::Files);
    foreach (QFileInfo fi, files) {
        ok &= QFile::remove(fi.absoluteFilePath());
    }

    QFileInfoList dirs = path.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo fi, dirs) {
        ok &= recursiveClearDir(fi.absoluteFilePath());
        ok &= QDir(fi.absoluteFilePath()).rmdir(".");
    }
    return ok;
}
