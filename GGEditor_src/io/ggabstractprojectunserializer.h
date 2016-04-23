#ifndef GGABSTRACTPROJECTUNSERIALIZER_H
#define GGABSTRACTPROJECTUNSERIALIZER_H

#include <QVariant>
#include <gg_definitions.h>

class GGProject;
class GGAbstractModel;
class GGScene;
class GGPage;
class GGConnection;
class QIODevice;

class GGAbstractProjectUnserializer
{
public:
    GGAbstractProjectUnserializer() {}
    virtual ~GGAbstractProjectUnserializer() {}

    virtual bool load(QIODevice *device) = 0;

    virtual GGProject *loadedProject() = 0;
    virtual GGProject *takeProject() = 0;

    virtual bool unserializeProject(QVariant project) = 0;
    virtual bool unserializeScene(QVariant scene) = 0;
    virtual bool unserializePage(QVariant page) = 0;
    virtual bool unserializeConnection(QVariant connection) = 0;

protected:
    void setSceneId(GGScene *sc, GG::SceneID id);

};

#endif // GGABSTRACTPROJECTUNSERIALIZER_H
