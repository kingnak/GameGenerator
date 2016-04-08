#ifndef GGRUNTIMEPROJECT_H
#define GGRUNTIMEPROJECT_H

#include <model/ggproject.h>

class GGRuntimeModel;

class GG_CORESHARED_EXPORT GGRuntimeProject : public GGProject
{
public:
    GGRuntimeProject(GGAbstractMediaResolver *resolver);
    ~GGRuntimeProject();

    virtual GGAbstractModel *model();
    virtual GGRuntimeModel *runtimeModel();
    virtual GGAbstractMediaResolver *resolver();

protected:
    GGRuntimeProject(GGRuntimeModel *model);
    GGRuntimeProject();

protected:
    GGRuntimeModel *m_runtimeModel;
};

#endif // GGRUNTIMEPROJECT_H
