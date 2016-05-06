#include "ggtrasher.h"

GGTrasher *GGTrasher::s_trasher = NULL;

GGTrasher *GGTrasher::trasher()
{
    return s_trasher;
}

void GGTrasher::setTrasher(GGTrasher *trasher)
{
    s_trasher = trasher;
}




QString GGDefaultTrasher::TRASH_DIR_NAME = "trash";

GGDefaultTrasher::GGDefaultTrasher()
{
    m_baseDir = QDir::temp().absoluteFilePath(TRASH_DIR_NAME);
}

void GGDefaultTrasher::setBaseDir(QDir dir)
{
    m_baseDir = dir;
}

QDir GGDefaultTrasher::baseDir() const
{
    return m_baseDir;
}

bool GGDefaultTrasher::moveToTrash(const QString &file)
{
    if (!m_baseDir.exists()) {
        if (!m_baseDir.mkpath(".")) {
            return false;
        }
    }

    if (!QFile::exists(file)) {
        return false;
    }

    QFileInfo fi(file);
    QString ext = fi.completeSuffix();
    if (!ext.isEmpty()) ext = "."+ext;
    QString fn = fi.baseName();

    // Find "free" file name
    int ct = 0;
    while (QFile::exists(m_baseDir.absoluteFilePath(fn + ext))) {
        fn = fi.baseName() + QString::number(++ct);
    }

    return QFile::rename(fi.absoluteFilePath(), m_baseDir.absoluteFilePath(fn+ext));
}
