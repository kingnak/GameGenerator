#ifndef GGSERIALIZATIONPROCESSOR_H
#define GGSERIALIZATIONPROCESSOR_H

#include <QVariantMap>
/*
class GGEditProject;
class GGScene;
class GGPage;
class GGConnection;
*/
class GGSerializationProcessor
{
public:
    virtual ~GGSerializationProcessor() {}

    virtual bool processProject(QVariant &project) = 0;
    virtual bool processScene(QVariant &scene) = 0;
    virtual bool processPage(QVariant &page) = 0;
    virtual bool processConnection(QVariant &connection) = 0;

    virtual bool processSceneRef(QVariant &sceneRef) = 0;
    virtual bool processConnectionRef(QVariant &connRef) = 0;
    virtual bool processMediaRef(QVariant &mediaRef) = 0;
    virtual bool processContentElement(QVariant &elem) = 0;
    virtual bool processMappedLink(QVariant &mappedLink) = 0;
    virtual bool processLink(QVariant &link) = 0;
};


class GGDefaultSerializationProcessor : public GGSerializationProcessor
{
public:
    virtual bool processProject(QVariant &) { return true; }
    virtual bool processScene(QVariant &) { return true; }
    virtual bool processPage(QVariant &) { return true; }
    virtual bool processConnection(QVariant &) { return true; }
    virtual bool processSceneRef(QVariant &) { return true; }
    virtual bool processConnectionRef(QVariant &) { return true; }
    virtual bool processMediaRef(QVariant &) { return true; }
    virtual bool processContentElement(QVariant &) { return true; }
    virtual bool processMappedLink(QVariant &) { return true; }
    virtual bool processLink(QVariant &) { return true; }
};

#endif // GGSERIALIZATIONPROCESSOR_H
