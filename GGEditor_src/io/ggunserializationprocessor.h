#ifndef GGUNSERIALIZATIONPROCESSOR_H
#define GGUNSERIALIZATIONPROCESSOR_H

#include <QVariant>

class GGUnserializationProcessor
{
public:
    virtual ~GGUnserializationProcessor() {}

    virtual bool processProject(QVariant &project) = 0;
    virtual bool processScene(QVariant &scene) = 0;
    virtual bool processPage(QVariant &page) = 0;
    virtual bool processConnection(QVariant &connection) = 0;
    virtual bool processMedia(QVariant &media) = 0;

    virtual bool processSceneRef(QVariant &sceneRef) = 0;
    virtual bool processPageRef(QVariant &pageRef) = 0;
    virtual bool processConnectionRef(QVariant &connRef) = 0;
    virtual bool processMediaRef(QVariant &mediaRef) = 0;
    virtual bool processContentElement(QVariant &elem) = 0;
    virtual bool processMappedLink(QVariant &mappedLink) = 0;
    virtual bool processLink(QVariant &link) = 0;
    virtual bool processVariable(QVariant &var) = 0;
    virtual bool processCondition(QVariant &cond) = 0;
    virtual bool processAction(QVariant &act) = 0;
};

class GGDefaultUnserializationProcessor : public GGUnserializationProcessor
{
public:
    virtual bool processProject(QVariant &) { return true; }
    virtual bool processScene(QVariant &) { return true; }
    virtual bool processPage(QVariant &) { return true; }
    virtual bool processConnection(QVariant &) { return true; }
    virtual bool processMedia(QVariant &) { return true; }
    virtual bool processSceneRef(QVariant &) { return true; }
    virtual bool processPageRef(QVariant &) { return true; }
    virtual bool processConnectionRef(QVariant &) { return true; }
    virtual bool processMediaRef(QVariant &) { return true; }
    virtual bool processContentElement(QVariant &) { return true; }
    virtual bool processMappedLink(QVariant &) { return true; }
    virtual bool processLink(QVariant &) { return true; }
    virtual bool processVariable(QVariant &) { return true; }
    virtual bool processCondition(QVariant &) { return true; }
    virtual bool processAction(QVariant &) { return true; }
};

#endif // GGUNSERIALIZATIONPROCESSOR_H

