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

    virtual bool processConnectionRef(QVariant &connRef) = 0;
    virtual bool processMediaRef(QVariant &mediaRef) = 0;
    virtual bool processContentElement(QVariant &elem) = 0;
    virtual bool processMappedLink(QVariant &mappedLink) = 0;
    virtual bool processLink(QVariant &link) = 0;
};

#endif // GGSERIALIZATIONPROCESSOR_H
