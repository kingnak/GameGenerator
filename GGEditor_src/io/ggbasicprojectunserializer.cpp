#include "ggbasicprojectunserializer.h"
#include <model/ggeditproject.h>
#include <io/ggserialization.hpp>
#include <io/ggabstractunserializationreader.h>
#include <io/ggunserializationprocessor.h>
#include <model/ggeditmodel.h>
#include <model/ggabstractfactory.h>
#include <model/ggscene.h>
#include <model/ggvariable.h>

GGBasicProjectUnserializer::GGBasicProjectUnserializer(const QString &basePath, const QString &fileName, GGAbstractUnserializationReader *reader, GGUnserializationProcessor *processor)
    : m_reader(reader),
      m_processor(processor),
      m_project(NULL)
{
    m_project = new GGEditProject(basePath, fileName, GGIOFactory::BinaryModel);
}

GGBasicProjectUnserializer::~GGBasicProjectUnserializer()
{
    delete m_project;
}

bool GGBasicProjectUnserializer::load(QIODevice *device)
{
    return m_reader->unserialize(device, this);
}

GGEditProject *GGBasicProjectUnserializer::loadedProject()
{
    return m_project;
}

GGEditProject *GGBasicProjectUnserializer::takeProject()
{
    GGEditProject *ret = m_project;
    m_project = NULL;
    return ret;
}

bool GGBasicProjectUnserializer::unserializeProject(QVariant project)
{
    if (!m_processor->processProject(project)) return false;

    if (!project.canConvert<QVariantMap>()) return false;

    QVariantMap map = project.value<QVariantMap>();
    if (!map.contains("title")) return false;
    m_project->setTitle(map["title"].toString());

    QVariantList lst;
    map["variable"] >> lst;
    bool ok = true;
    foreach (QVariant v, lst) {
        GGVariable var;
        ok &= unserializeVariable(v, var);
        m_project->editModel()->addVariable(var);
    }

    return ok;
}

bool GGBasicProjectUnserializer::unserializeScene(QVariant scene)
{
    if (!m_processor->processScene(scene)) return false;
    if (!scene.canConvert<QVariantMap>()) return false;
    QVariantMap map = scene.value<QVariantMap>();

    if (!map.contains("id") || !map.contains("name")) return false;

    GGScene *sc = m_project->editModel()->factory()->createScene();
    sc->setName(map["name"].toString());
    sc->setMediaDir(map["mediaDir"].toString());
    bool ok;
    GG::SceneID id = static_cast<GG::SceneID> (map["id"].toUInt(&ok));

    this->setSceneId(sc, id);
    ok &= m_project->model()->registerSceneWithId(sc);

    return ok;
}

bool GGBasicProjectUnserializer::unserializePage(QVariant page)
{
    return true;
}

bool GGBasicProjectUnserializer::unserializeConnection(QVariant connection)
{
    return true;
}

bool GGBasicProjectUnserializer::unserializeVariable(QVariant data, GGVariable &var)
{
    if (!data.canConvert<QVariantMap>()) return false;
    QVariantMap map = data.value<QVariantMap>();
    if (!map.contains("name") || !map.contains("type")) {
        return false;
    }

    var = GGVariable(map["name"].toString(), map["initial"].toString(), static_cast<GGVariable::Type>(map["type"].toUInt()));
    return true;
}
