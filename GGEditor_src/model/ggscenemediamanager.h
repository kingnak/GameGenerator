#ifndef GGSCENEMEDIAMANAGER_H
#define GGSCENEMEDIAMANAGER_H

#include <model/ggmediamanager.h>

class GGEditModel;
class GGScene;

class GGSceneMediaManager : public GGMediaManager
{
public:
    GGSceneMediaManager(GGEditModel *model, const QDir &baseDir);

    QString checkIn(GGScene *scene, const QString &file, bool moveFile = false);

protected:
    virtual QString getCheckInPath(const QString &file);

private:
    GGEditModel *m_model;
    GGScene *m_checkInScene;
};

#endif // GGSCENEMEDIAMANAGER_H
