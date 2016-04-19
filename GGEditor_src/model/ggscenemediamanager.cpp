#include "ggscenemediamanager.h"
#include <model/ggeditmodel.h>
#include <model/ggscene.h>

GGSceneMediaManager::GGSceneMediaManager(GGEditModel *model, const QDir &baseDir, QObject *parent)
    : GGMediaManager(baseDir),
      QObject(parent),
      m_model(model),
      m_checkInScene(NULL)
{
    connect(m_model, SIGNAL(sceneRegistered(GGScene*)), this, SLOT(sceneRegistered(GGScene*)));
    connect(m_model, SIGNAL(sceneUnregistered(GG::SceneID,GGScene*)), this, SLOT(sceneUnregistered(GG::SceneID)));
    connect(m_model, SIGNAL(sceneUpdated(GGScene*)), this, SLOT(sceneUpdated(GGScene*)));
}

QString GGSceneMediaManager::checkIn(GGScene *scene, const QString &file, bool moveFile)
{
    m_checkInScene = scene;
    QString ret = GGMediaManager::checkIn(file, moveFile);
    m_checkInScene = NULL;
    return ret;
}

void GGSceneMediaManager::synchronize()
{
    m_sceneDirNames.clear();
    foreach (GGScene *s, m_model->getScenes()) {
        m_sceneDirNames[s->id()] = s->mediaDir();
    }
    GGMediaManager::synchronize();
}

QString GGSceneMediaManager::getDisplayString(const QString &path, int level)
{
    if (level == 1) {
        return GGMediaManager::getDisplayString(path, 0);
    }
    if (level == 0) {
        // Check if it is a scene's dir
        foreach (GGScene *s, m_model->getScenes()) {
            if (path == s->mediaDir()) {
                return s->name();
            }
        }
    }
    return path;
}

QString GGSceneMediaManager::getCheckInPath(const QString &file)
{
    QString dir = GGMediaManager::getCheckInPath(file);
    if (m_checkInScene && !m_checkInScene->mediaDir().isEmpty()) {
        dir = m_checkInScene->mediaDir() + "/" + dir;
    }
    return dir;
}

QStringList GGSceneMediaManager::getDefaultMediaPaths()
{
    QStringList defaults = GGMediaManager::getDefaultMediaPaths();
    QStringList ret;
    foreach (GGScene *s, m_model->getScenes()) {
        foreach (QString d, defaults) {
            ret << s->mediaDir() + "/" + d;
        }
    }
    return ret;
}

void GGSceneMediaManager::sceneRegistered(GGScene *scene)
{
    createSceneDirs(scene);
}

void GGSceneMediaManager::sceneUnregistered(GG::SceneID id)
{
    m_sceneDirNames.remove(id);
}

void GGSceneMediaManager::sceneUpdated(GGScene *scene)
{
    if (scene->mediaDir() != m_sceneDirNames.value(scene->id())) {
        renameSceneDir(scene);
    }
}

void GGSceneMediaManager::createSceneDirs(GGScene *scene)
{
    QStringList defaults = GGMediaManager::getDefaultMediaPaths();
    foreach (QString d, defaults) {
        m_baseDir.mkpath(scene->mediaDir() + "/" + d);
    }
    m_sceneDirNames[scene->id()] = scene->mediaDir();
    synchDir(m_baseDir);
}

void GGSceneMediaManager::renameSceneDir(GGScene *scene)
{
    if (m_sceneDirNames.contains(scene->id()) && m_sceneDirNames[scene->id()] != scene->mediaDir()) {
        if (m_baseDir.rename(m_sceneDirNames[scene->id()], scene->mediaDir())) {
            m_sceneDirNames[scene->id()] = scene->mediaDir();
            synchDir(m_baseDir);
        }
    }
}
