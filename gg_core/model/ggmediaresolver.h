#ifndef GGMEDIARESOLVER_H
#define GGMEDIARESOLVER_H

#include <gg_core_global.h>
#include <QDir>

class QIODevice;

class GG_CORESHARED_EXPORT GGAbstractMediaResolver
{
public:
    virtual ~GGAbstractMediaResolver() {}

    virtual QIODevice *resolve(const QString &media) = 0;
};

//////////////////

class GG_CORESHARED_EXPORT GGFileSystemResolver : public GGAbstractMediaResolver
{
public:
    GGFileSystemResolver(const QDir &base = QDir::current());
    void setBase(const QDir &base);
    QDir getBase() const;

    virtual QIODevice *resolve(const QString &media);

private:
    QDir m_base;
};

#endif // GGMEDIARESOLVER_H

