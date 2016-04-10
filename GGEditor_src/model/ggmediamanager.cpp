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

QStringList GGMediaManager::imageSuffixes()
{
    // TODO: Extend? Resolve by Qt Image Plugins? Sensible web formats?
    return QStringList() << "jpg" << "jpeg" << "gif" << "png";
}

QStringList GGMediaManager::videoSuffixes()
{
    // TODO
    return QStringList();
}

QStringList GGMediaManager::audioSuffixes()
{
    // TODO
    return QStringList() << "mp3";
}

QDir GGMediaManager::baseDir() const
{
    return m_baseDir;
}

QStringList GGMediaManager::allMedia() const
{
    return m_path2id.keys();
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
    QStringList ret;
    foreach (QString s, m_path2id.keys()) {
        QFileInfo f(m_baseDir.absoluteFilePath(s));
        if (!f.exists()) {
            ret << s;
        }
    }
    return ret;
}

void GGMediaManager::synchronize()
{
    for (QMap<QString, QString>::iterator it = m_path2id.begin(); it != m_path2id.end(); ++it) {
        QFileInfo f(m_baseDir.absoluteFilePath(it.key()));
        if (!f.exists()) {
            qDebug() << "GGMediaManager Synchronize: removing" << it.key();
            m_id2path.remove(it.value());
            it = m_path2id.erase(it);
        }
    }

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

bool GGMediaManager::isFileManaged(const QString &file)
{
    return m_path2id.contains(toManagedPath(file));
}

bool GGMediaManager::isFilePathInManager(const QString &file)
{
    QString relPath = toManagedPath(file);
    if (relPath.isNull()) {
        return false;
    }
    return true;
}

QString GGMediaManager::toManagedPath(const QString &file)
{
    QFileInfo f(file);
    if (!f.exists()) {
        return QString::null;
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
    if (imageSuffixes().contains(ext)) {
        dir = PATH_IMAGE;
    } else if (audioSuffixes().contains(ext)) {
        dir = PATH_AUDIO;
    } else if (videoSuffixes().contains(ext)) {
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
        synchDir(dir.absoluteFilePath(s));
    }
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
