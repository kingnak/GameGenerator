#include "ggeditproject.h"
#include <model/ggeditmodel.h>
#include <model/ggmediaresolver.h>
#include <model/ggsimplefactory.h>
#include <model/ggscenemediamanager.h>

GGEditProject::GGEditProject(const QString &basePath)
{
    m_editModel = new GGEditModel(new GGSimpleFactory, NULL);
    m_manager = new GGSceneMediaManager(m_editModel, basePath);
    m_editModel->setMediaResolver(m_manager->resolver());
    m_runtimeModel = m_editModel;
}

GGEditModel *GGEditProject::editModel()
{
    return m_editModel;
}

GGMediaManager *GGEditProject::mediaManager()
{
    return m_manager;
}

