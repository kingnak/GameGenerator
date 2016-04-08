#include "ggeditproject.h"
#include <model/ggeditmodel.h>
#include <model/ggmediaresolver.h>
#include <model/ggsimplefactory.h>

GGEditProject::GGEditProject()
{
    m_resolver = new GGFileSystemResolver;
    m_editModel = new GGEditModel(new GGSimpleFactory, m_resolver);
    m_runtimeModel = m_editModel;
}

void GGEditProject::setBasePath(const QString &basePath)
{
    static_cast<GGFileSystemResolver *> (m_resolver)->setBase(basePath);
}

GGEditModel *GGEditProject::editModel()
{
    return m_editModel;
}

