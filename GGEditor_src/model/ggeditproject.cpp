#include "ggeditproject.h"
#include <model/ggeditmodel.h>
#include <model/ggmediaresolver.h>
#include <model/ggsimplefactory.h>
#include <model/ggscenemediamanager.h>

GGEditProject::GGEditProject(const QString &basePath)
    : GGRuntimeProject(new GGEditModel(new GGSimpleFactory))
{
    m_basePath.setPath(basePath);
    m_editModel = static_cast<GGEditModel *> (m_runtimeModel);
    m_manager = new GGSceneMediaManager(m_editModel, basePath);
    m_editModel->setMediaResolver(m_manager->resolver());
}

GGEditProject::~GGEditProject()
{
    delete m_manager;
}

GGEditModel *GGEditProject::editModel()
{
    return m_editModel;
}

GGSceneMediaManager *GGEditProject::mediaManager()
{
    return m_manager;
}

QDir GGEditProject::basePath() const
{
    return m_basePath;
}

