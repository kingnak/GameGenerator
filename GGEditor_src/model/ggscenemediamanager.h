#ifndef GGSCENEMEDIAMANAGER_H
#define GGSCENEMEDIAMANAGER_H

#include <model/ggmediamanager.h>
#include <gg_definitions.h>
#include <QMap>

class GGEditModel;
class GGScene;

class GGSceneMediaManager : public QObject, public GGMediaManager
{
    Q_OBJECT
public:
    GGSceneMediaManager(GGEditModel *model, const QDir &baseDir, QObject *parent = 0);

    QString checkIn(GGScene *scene, const QString &file, bool moveFile = false);

    virtual void synchronize();
    virtual QString getDisplayString(const QString &path, int level);
    GG::SceneID getSceneForPath(const QString &path, int level);

    GGEditModel *model();

protected:
    virtual QString getCheckInPath(const QString &file);

    virtual QStringList getDefaultMediaPaths();

private slots:
    void sceneRegistered(GGScene *scene);
    void sceneUnregistered(GG::SceneID id);
    void sceneUpdated(GGScene *scene);

private:
    void createSceneDirs(GGScene *scene);
    void renameSceneDir(GGScene *scene);

private:
    GGEditModel *m_model;
    GGScene *m_checkInScene;
    QMap<GG::SceneID, QString> m_sceneDirNames;
};

#endif // GGSCENEMEDIAMANAGER_H
