#ifndef GGEDITPROJECT_H
#define GGEDITPROJECT_H

#include <model/ggruntimeproject.h>

class GGEditModel;

class GGEditProject : public GGRuntimeProject
{
public:
    GGEditProject();

    GGEditModel *editModel();

private:
    GGEditModel *m_editModel;
    GGAbstractMediaResolver *m_resolver;
};

#endif // GGEDITPROJECT_H
