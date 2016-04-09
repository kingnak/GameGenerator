#include "ggscenemediamanager.h"
#include <model/ggeditmodel.h>
#include <model/ggscene.h>

GGSceneMediaManager::GGSceneMediaManager(GGEditModel *model, const QDir &baseDir)
    : GGMediaManager(baseDir),
      m_model(model),
      m_checkInScene(NULL)
{

}

QString GGSceneMediaManager::checkIn(GGScene *scene, const QString &file, bool moveFile)
{
    m_checkInScene = scene;
    QString ret = GGMediaManager::checkIn(file, moveFile);
    m_checkInScene = NULL;
    return ret;
}

QString GGSceneMediaManager::getCheckInPath(const QString &file)
{
    QString dir = GGMediaManager::getCheckInPath(file);
    if (m_checkInScene && !m_checkInScene->mediaDir().isEmpty()) {
        dir = m_checkInScene->mediaDir() + "/" + dir;
    }
    return dir;
}

