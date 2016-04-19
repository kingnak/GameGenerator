#ifndef GGEDITPROJECT_H
#define GGEDITPROJECT_H

#include <model/ggruntimeproject.h>

class GGEditModel;
class GGSceneMediaManager;

class GGEditProject : public GGRuntimeProject
{
public:
    GGEditProject(const QString &basePath);
    ~GGEditProject();

    GGEditModel *editModel();
    GGSceneMediaManager *mediaManager();

private:
    GGEditModel *m_editModel;
    GGSceneMediaManager *m_manager;
};

#endif // GGEDITPROJECT_H
