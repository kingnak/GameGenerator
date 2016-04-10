#ifndef GGEDITPROJECT_H
#define GGEDITPROJECT_H

#include <model/ggruntimeproject.h>

class GGEditModel;
class GGMediaManager;

class GGEditProject : public GGRuntimeProject
{
public:
    GGEditProject(const QString &basePath);

    GGEditModel *editModel();
    GGMediaManager *mediaManager();

private:
    GGEditModel *m_editModel;
    GGMediaManager *m_manager;
};

#endif // GGEDITPROJECT_H
