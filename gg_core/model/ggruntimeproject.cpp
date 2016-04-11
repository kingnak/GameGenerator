#include "ggruntimeproject.h"
#include <model/ggruntimemodel.h>

GGRuntimeProject::GGRuntimeProject()
{
    m_runtimeModel = new GGRuntimeModel;
}

GGRuntimeProject::GGRuntimeProject(GGRuntimeModel *model)
    : m_runtimeModel(model)
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
