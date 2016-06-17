#ifndef GGMEDIARESOLVER_H
#define GGMEDIARESOLVER_H

#include <gg_core_global.h>
#include <QDir>

class QIODevice;

class GG_CORESHARED_EXPORT GGAbstractMediaResolver
{
public:
    virtual ~GGAbstractMediaResolver() {}

    virtual QIODevice *resolve(const QString &media) const = 0;
    virtual QString resolveName(const QString &media) const = 0;
    virtual QString resolveTypeHint(const QString &media) const = 0;
    virtual QString resolveFile(const QString &media) const = 0;
    virtual bool isValid(const QString &media) const = 0;
};

//////////////////

class GG_CORESHARED_EXPORT GGFileSystemResolver : public GGAbstractMediaResolver
{
public:
    GGFileSystemResolver(const QDir &base = QDir::current());
    void setBase(const QDir &base);
    QDir getBase() const;

    virtual QIODevice *resolve(const QString &media) const;
    virtual QString resolveName(const QString &media) const;
    virtual QString resolveTypeHint(const QString &media) const;
    virtual QString resolveFile(const QString &media) const;
    virtual bool isValid(const QString &media) const;

private:
    QDir m_base;
};

#endif // GGMEDIARESOLVER_H

