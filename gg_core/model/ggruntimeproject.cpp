#include "ggruntimeproject.h"
#include <model/ggruntimemodel.h>

GGRuntimeProject::GGRuntimeProject(GGAbstractMediaResolver *resolver)
{
    m_runtimeModel = new GGRuntimeModel(resolver);
}

GGRuntimeProject::GGRuntimeProject(GGRuntimeModel *model)
    : m_runtimeModel(model)
{

}

GGRuntimeProject::GGRuntimeProject()
    : m_runtimeModel(NULL)
{
}

GGRuntimeProject::~GGRuntimeProject()
{
    delete m_runtimeModel;
}

GGAbstractModel *GGRuntimeProject::model()
{
    return m_runtimeModel;
}

GGRuntimeModel *GGRuntimeProject::runtimeModel()
{
    return m_runtimeModel;
}

GGAbstractMediaResolver *GGRuntimeProject::resolver()
{
    return m_runtimeModel->mediaResolver();
}
