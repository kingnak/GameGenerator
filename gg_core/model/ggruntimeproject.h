#ifndef GGRUNTIMEPROJECT_H
#define GGRUNTIMEPROJECT_H

#include <model/ggproject.h>

class GGRuntimeModel;

class GG_CORESHARED_EXPORT GGRuntimeProject : public GGProject
{
public:
    GGRuntimeProject();
    ~GGRuntimeProject();

    virtual GGAbstractModel *model();
    virtual const GGAbstractModel *model() const;
    virtual GGRuntimeModel *runtimeModel();
    virtual GGAbstractMediaResolver *resolver();
    virtual const GGAbstractMediaResolver *resolver() const;

protected:
    GGRuntimeProject(GGRuntimeModel *model);

protected:
    GGRuntimeModel *m_runtimeModel;
};

#endif // GGRUNTIMEPROJECT_H
