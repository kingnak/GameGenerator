#include "ggmediaresolver.h"

GGFileSystemResolver::GGFileSystemResolver(const QDir &base)
    : m_base(base)
{

}

void GGFileSystemResolver::setBase(const QDir &base)
{
    m_base = base;
}

QDir GGFileSystemResolver::getBase() const
{
    return m_base;
}

QIODevice *GGFileSystemResolver::resolve(const QString &media) const
{
    QString path = m_base.absoluteFilePath(media);
    if (QFile::exists(path)) {
        QFile *f = new QFile(path);
        if (f->open(QIODevice::ReadOnly)) {
            return f;
        }
        delete f;
    }
    return NULL;
}

QString GGFileSystemResolver::resolveName(const QString &media) const
{
    QString path = m_base.absoluteFilePath(media);
    return path;
}

QString GGFileSystemResolver::resolveTypeHint(const QString &media) const
{
    QFileInfo f(m_base.absoluteFilePath(media));
    return f.completeSuffix();
}

QString GGFileSystemResolver::resolveFile(const QString &media) const
{
    return m_base.absoluteFilePath(media);
}

bool GGFileSystemResolver::isValid(const QString &media) const
{
    return QFile::exists(m_base.absoluteFilePath(media));
}

