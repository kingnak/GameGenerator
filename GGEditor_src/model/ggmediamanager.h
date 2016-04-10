#ifndef GGMEDIAMANAGER_H
#define GGMEDIAMANAGER_H

#include <QDir>
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

    bool init();
    QStringList verify();
    void synchronize();
    QString checkIn(const QString &file, bool moveFile = false);

    bool isFileManaged(const QString &file);
    bool isFilePathInManager(const QString &file);

    GGMediaManagerResolver *resolver() { return m_resolver; }

protected:
    QString toManagedPath(const QString &file);
    virtual QString getCheckInPath(const QString &file);

    void synchDir(QDir dir);

protected:
    QDir m_baseDir;
    QMap<QString, QString> m_id2path;
    QMap<QString, QString> m_path2id;

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

private:
    GGMediaManager *m_manager;
};

#endif // GGMEDIAMANAGER_H
