#ifndef GGABSTRACTPROJECTUNSERIALIZER_H
#define GGABSTRACTPROJECTUNSERIALIZER_H

#include <QVariant>
#include <gg_definitions.h>

class GGProject;
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
    virtual bool finalizeUnserialization() = 0;

protected:
    void setSceneId(GGScene *sc, GG::SceneID id);
    void setPageId(GGPage *p, GG::PageID id);
    void setConnectionId(GGConnection *c, GG::ConnectionID id);
    void resolveConnectionPages(GGConnection *conn, GGPage *source, GGPage *dest);
};

#endif // GGABSTRACTPROJECTUNSERIALIZER_H
