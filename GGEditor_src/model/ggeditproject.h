#ifndef GGEDITPROJECT_H
#define GGEDITPROJECT_H

#include <model/ggruntimeproject.h>
#include <QDir>

class GGEditModel;
class GGSceneMediaManager;

class GGEditProject : public GGRuntimeProject
{
public:
    GGEditProject(const QString &basePath);
    ~GGEditProject();

    GGEditModel *editModel();
    GGSceneMediaManager *mediaManager();
    QDir basePath() const;

private:
    GGEditModel *m_editModel;
    GGSceneMediaManager *m_manager;
    QDir m_basePath;
};

#endif // GGEDITPROJECT_H
