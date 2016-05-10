#include "ggeditproject.h"
#include <model/ggeditmodel.h>
#include <model/ggmediaresolver.h>
#include <model/ggsimplefactory.h>
#include <model/ggscenemediamanager.h>
#include <style/ggspanstyler.h>

GGEditProject::GGEditProject(const QString &basePath, const QString &fileName, GGIOFactory::SerializationType saveType)
    : GGRuntimeProject(new GGEditModel(new GGSimpleFactory))
{
    m_basePath.setPath(basePath);
    m_fileName = fileName;
    m_saveType = saveType;
    m_editModel = static_cast<GGEditModel *> (m_runtimeModel);
    m_manager = new GGSceneMediaManager(m_editModel, basePath);
    m_editModel->setMediaResolver(m_manager->resolver());
    m_editModel->setStyler(new GGSpanStyler);
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

QString GGEditProject::fileName() const
{
    return m_fileName;
}

GGIOFactory::SerializationType GGEditProject::saveType() const
{
    return m_saveType;
}

void GGEditProject::setSaveType(GGIOFactory::SerializationType type)
{
    m_saveType = type;
}

