#ifndef GGMEDIAMANAGER_H
#define GGMEDIAMANAGER_H

#include <QDir>
#include <QSet>
#include <QMap>
#include <QString>
#include <QStringList>

#include <model/ggmediaresolver.h>

class GGMediaManagerResolver;

class GGMediaManager
{
public:
    static const QString PATH_BASE;
    static const QString PATH_IMAGE;
    static const QString PATH_AUDIO;
    static const QString PATH_VIDEO;
    static const QString PATH_OTHER;

    GGMediaManager(const QDir &baseDir);
    virtual ~GGMediaManager();

    static QStringList imageSuffixes();
    static QStringList videoSuffixes();
    static QStringList audioSuffixes();

    QDir baseDir() const;
    QStringList allMedia() const;
    QStringList allMediaWithDirs() const;

    bool init();
    QStringList verify();
    virtual void synchronize();
    QString checkIn(const QString &file, bool moveFile = false);

    bool isFileManaged(const QString &file) const;
    bool isFilePathInManager(const QString &file) const;
    QString getIdForFilePath(const QString &file) const;

    GGMediaManagerResolver *resolver() { return m_resolver; }

    virtual QString getDisplayString(const QString &path, int level);

protected:
    QString toManagedPath(const QString &file) const;
    virtual QString getCheckInPath(const QString &file);

    void synchDir(QDir dir);
    virtual QStringList getDefaultMediaPaths();

protected:
    QDir m_baseDir;
    QMap<QString, QString> m_id2path;
    QMap<QString, QString> m_path2id;
    QSet<QString> m_dirs;

    quint32 m_nextMediaId;
    static const quint32 INVALID_MEDIA_ID = 0;

    GGMediaManagerResolver *m_resolver;

    friend class GGMediaManagerResolver;
};

/////////////////

class GGMediaManagerResolver : public GGFileSystemResolver
{
public:
    GGMediaManagerResolver(GGMediaManager *manager);

    virtual QIODevice *resolve(const QString &media);
    virtual QString resolveName(const QString &media);
    virtual QString resolveTypeHint(const QString &media);
    virtual bool isValid(const QString &media);

private:
    GGMediaManager *m_manager;
};

#endif // GGMEDIAMANAGER_H
