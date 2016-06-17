#ifndef GGMEDIAMANAGER_H
#define GGMEDIAMANAGER_H

#include <QDir>
#include <QSet>
#include <QMap>
#include <QString>
#include <QStringList>

#include <model/ggmediaresolver.h>

class GGAbstractProjectUnserializer;
class GGMediaManagerResolver;

class GGMediaManager
{
public:
    enum MediaType {
        Other, Image, Audio, Video
    };

    GGMediaManager(const QDir &baseDir);
    virtual ~GGMediaManager();

    static QStringList getMediaSuffixes(MediaType type);
    static QString getBasePathName();
    static QString getMediaPathName(MediaType type);

    QDir baseDir() const;
    QStringList allMedia() const;
    QStringList allMediaWithDirs() const;

    bool init();
    QStringList verify();
    QStringList cleanUp();
    virtual void synchronize();
    QString checkIn(const QString &file, bool moveFile = false);
    bool removeManagedFile(const QString &file);

    bool isFileManaged(const QString &file) const;
    bool isFilePathInManager(const QString &file) const;
    QString getIdForFilePath(const QString &file) const;

    GGMediaManagerResolver *resolver() { return m_resolver; }

    virtual QString getDisplayString(const QString &path, int level);

    virtual MediaType getMediaTypeForPath(const QString &path, int level);

    virtual void synchronizeNextMediaId();

    QString getMediaSourceDir() const;
    void setMediaSourceDir(const QString &mediaSourceDir);

protected:
    QString toManagedPath(const QString &file, bool verifyExists = true) const;
    virtual QString getCheckInPath(const QString &file);
    QStringList verify(bool cleanUp);

    virtual QStringList getDefaultMediaPaths();
    void resynchBaseDir();

    virtual bool injectMedia(const QString &id, const QString &path);

private:
    void synchDir(QDir dir);

    friend class GGAbstractProjectUnserializer;

protected:
    static const QString PATH_BASE;
    static const QString PATH_IMAGE;
    static const QString PATH_AUDIO;
    static const QString PATH_VIDEO;
    static const QString PATH_OTHER;

protected:
    QDir m_baseDir;
    QMap<QString, QString> m_id2path;
    QMap<QString, QString> m_path2id;
    QSet<QString> m_dirs;
    QString m_mediaSourceDir;

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

    virtual QIODevice *resolve(const QString &media) const;
    virtual QString resolveName(const QString &media) const;
    virtual QString resolveTypeHint(const QString &media) const;
    virtual QString resolveFile(const QString &media) const;
    virtual bool isValid(const QString &media) const;

private:
    GGMediaManager *m_manager;
};

#endif // GGMEDIAMANAGER_H
