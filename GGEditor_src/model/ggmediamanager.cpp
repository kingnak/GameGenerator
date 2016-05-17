#include "ggmediamanager.h"
#include <QDebug>

const QString GGMediaManager::PATH_BASE("media");
const QString GGMediaManager::PATH_IMAGE("images");
const QString GGMediaManager::PATH_AUDIO("sounds");
const QString GGMediaManager::PATH_VIDEO("videos");
const QString GGMediaManager::PATH_OTHER("other");

GGMediaManager::GGMediaManager(const QDir &baseDir)
    : m_baseDir(baseDir), m_nextMediaId(0)
{
    init();
    m_resolver = new GGMediaManagerResolver(this);
}

GGMediaManager::~GGMediaManager()
{
    delete m_resolver;
}

QStringList GGMediaManager::getMediaSuffixes(GGMediaManager::MediaType type)
{
    switch (type) {
    case Image:
        // TODO: Extend? Resolve by Qt Image Plugins? Sensible web formats?
        return QStringList() << "jpg" << "jpeg" << "gif" << "png";
    case Audio:
        // TODO
        return QStringList() << "mp3";
    case Video:
        // TODO
        return QStringList() << "mpg" << "mpeg";
    case Other:
        return QStringList() << "";
    }
    return QStringList();
}

QString GGMediaManager::getBasePathName()
{
    return PATH_BASE;
}

QString GGMediaManager::getMediaPathName(GGMediaManager::MediaType type)
{
    switch (type) {
    case Image:
        return PATH_IMAGE;
    case Audio:
        return PATH_AUDIO;
    case Video:
        return PATH_VIDEO;
    case Other:
        return PATH_OTHER;
    }
    return PATH_OTHER;
}

QDir GGMediaManager::baseDir() const
{
    return m_baseDir;
}

QStringList GGMediaManager::allMedia() const
{
    return m_path2id.keys();
}

QStringList GGMediaManager::allMediaWithDirs() const
{
    return m_dirs.toList() + allMedia();
}

bool GGMediaManager::init()
{
    if (!m_baseDir.mkpath(PATH_BASE)) {
        return false;
    }
    if (!m_baseDir.cd(PATH_BASE)) {
        return false;
    }
    return true;
}

QStringList GGMediaManager::verify()
{
    return verify(false);
}

QStringList GGMediaManager::cleanUp()
{
    return verify(true);
}

QStringList GGMediaManager::verify(bool cleanUp)
{
    QStringList ret;
    QMap<QString, QString>::iterator it = m_path2id.begin();
    while (it != m_path2id.end()) {
        QFileInfo f(m_baseDir.absoluteFilePath(it.key()));
        if (!f.exists()) {
            ret << it.key();
            if (cleanUp) {
                m_id2path.remove(it.value());
                it = m_path2id.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    return ret;
}

void GGMediaManager::synchronize()
{
    QStringList dirs = getDefaultMediaPaths();
    foreach (QString d, dirs) {
        m_baseDir.mkpath(d);
    }

    m_dirs.clear();
    synchDir(m_baseDir);
}

QString GGMediaManager::checkIn(const QString &file, bool moveFile)
{
    QFileInfo fi(file);
    if (!fi.exists()) {
        return QString::null;
    }
    if (isFileManaged(file)) {
        return m_path2id[toManagedPath(file)];
    }

    QString checkinPath;
    if (!isFilePathInManager(file)) {
        // Copy/Move
        QString dest = m_baseDir.absoluteFilePath(getCheckInPath(fi.absoluteFilePath()));
        if (dest.isEmpty()) {
            return QString::null;
        }

        if (QFile::exists(dest)) {
            qWarning() << "GGMediaManager: Destination file" << dest << "already exists";
            return QString::null;
        }

        // Create directory if necessary
        QFileInfo destInfo(dest);
        destInfo.absoluteDir().mkpath(".");

        if (moveFile) {
            if (!QFile::rename(fi.absoluteFilePath(), dest)) {
                return QString::null;
            }
        } else {
            if (!QFile::copy(fi.absoluteFilePath(), dest)) {
                return QString::null;
            }
        }
        checkinPath = toManagedPath(dest);
    } else {
        // Already in manager dir, but not managed. Do nothing...
        checkinPath = toManagedPath(fi.absoluteFilePath());
    }

    Q_ASSERT_X(!checkinPath.isEmpty(), "GGMediaManager::checkIn", "Check In Path is empty");
    if (checkinPath.isEmpty()) {
        return QString::null;
    }

    quint32 id = ++m_nextMediaId;
    QString sId = QString::number(id);
    m_id2path[sId] = checkinPath;
    m_path2id[checkinPath] = sId;
    return sId;
}

bool GGMediaManager::removeManagedFile(const QString &file)
{
    if (!isFileManaged(file)) return false;
    QString managed = toManagedPath(file, false);
    QString id = m_path2id[managed];
    m_id2path.remove(id);
    m_path2id.remove(managed);
    return true;
}

bool GGMediaManager::isFileManaged(const QString &file) const
{
    return m_path2id.contains(toManagedPath(file));
}

bool GGMediaManager::isFilePathInManager(const QString &file) const
{
    QString relPath = toManagedPath(file, false);
    if (relPath.isNull()) {
        return false;
    }
    return true;
}

QString GGMediaManager::getIdForFilePath(const QString &file) const
{
    return m_path2id.value(file);
}

QString GGMediaManager::getDisplayString(const QString &path, int level)
{
    if (level == 0) {
        if (path == PATH_IMAGE) {
            return "Images";
        } else if (path == PATH_AUDIO) {
            return "Sounds";
        } else if (path == PATH_VIDEO) {
            return "Videos";
        }
    }
    return path;
}

GGMediaManager::MediaType GGMediaManager::getMediaTypeForPath(const QString &path, int level)
{
    if (level == 0) {
        if (path == PATH_IMAGE) return Image;
        if (path == PATH_VIDEO) return Video;
        if (path == PATH_AUDIO) return Audio;
    }
    return Other;
}

void GGMediaManager::synchronizeNextMediaId()
{
    quint32 nextMedia = INVALID_MEDIA_ID;
    foreach (QString id, m_id2path.keys()) {
        quint32 nId = id.toUInt();
        if (nId > nextMedia) nextMedia = nId;
    }
    if (m_nextMediaId < nextMedia) {
        m_nextMediaId = nextMedia;
    }
}

QString GGMediaManager::toManagedPath(const QString &file, bool verifyExists) const
{
    QFileInfo f(file);
    if (verifyExists) {
        if (!f.exists()) {
            return QString::null;
        }
    }

    QString fPath = f.canonicalFilePath();
    QString bPath = m_baseDir.canonicalPath();

    if (fPath.startsWith(bPath+"/")) {
        QString rel = QDir::fromNativeSeparators(m_baseDir.relativeFilePath(fPath));
        if (rel.startsWith("..")) {
            // Somehow we got it wrong...
            Q_ASSERT(false);
            return QString::null;
        }
        return rel;
    }
    return QString::null;
}

QString GGMediaManager::getCheckInPath(const QString &file)
{
    QFileInfo fi(file);
    QString ext = fi.suffix().toLower();
    QString dir;
    if (getMediaSuffixes(Image).contains(ext)) {
        dir = PATH_IMAGE;
    } else if (getMediaSuffixes(Audio).contains(ext)) {
        dir = PATH_AUDIO;
    } else if (getMediaSuffixes(Video).contains(ext)) {
        dir = PATH_VIDEO;
    } else {
        dir = PATH_OTHER;
    }

    return dir + "/" + fi.fileName();
}

void GGMediaManager::synchDir(QDir dir)
{
    QStringList lst = dir.entryList(QDir::Files);
    foreach (QString s, lst) {
        checkIn(dir.absoluteFilePath(s));
    }

    lst = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString s, lst) {
        m_dirs << m_baseDir.relativeFilePath(dir.absoluteFilePath(s));
        synchDir(dir.absoluteFilePath(s));
    }
}

QString GGMediaManager::getMediaSourceDir() const
{
    return m_mediaSourceDir;
}

void GGMediaManager::setMediaSourceDir(const QString &mediaSourceDir)
{
    m_mediaSourceDir = mediaSourceDir;
}

QStringList GGMediaManager::getDefaultMediaPaths()
{
    return QStringList() << PATH_IMAGE << PATH_AUDIO << PATH_VIDEO;
}

void GGMediaManager::resynchBaseDir()
{
    m_dirs.clear();
    synchDir(m_baseDir);
}

bool GGMediaManager::injectMedia(const QString &id, const QString &path)
{
    if (m_id2path.contains(id)) return false;
    if (m_path2id.contains(path)) return false;
    if (!isFilePathInManager(m_baseDir.absoluteFilePath(path))) return false;

    bool ok;
    if (id.toUInt(&ok) == INVALID_MEDIA_ID) return false;
    if (!ok) return false;

    m_id2path[id] = path;
    m_path2id[path] = id;
    return true;
}

/////////////////

GGMediaManagerResolver::GGMediaManagerResolver(GGMediaManager *manager)
    : GGFileSystemResolver(manager->m_baseDir),
      m_manager(manager)
{

}

QIODevice *GGMediaManagerResolver::resolve(const QString &media)
{
    QString path = m_manager->m_id2path[media];
    return GGFileSystemResolver::resolve(path);
}

QString GGMediaManagerResolver::resolveName(const QString &media)
{
    QString path = m_manager->m_id2path[media];
    return path;
}

QString GGMediaManagerResolver::resolveTypeHint(const QString &media)
{
    QString path = m_manager->m_id2path[media];
    return GGFileSystemResolver::resolveTypeHint(path);
}

bool GGMediaManagerResolver::isValid(const QString &media)
{
    return m_manager->m_id2path.contains(media);
}
