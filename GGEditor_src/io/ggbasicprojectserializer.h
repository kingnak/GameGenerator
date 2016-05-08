#ifndef GGBASICPROJECTSERIALIZER_H
#define GGBASICPROJECTSERIALIZER_H

#include <QVariantMap>

class GGAbstractSerializationWriter;
class GGSerializationProcessor;
class GGEditProject;
class GGScene;
class GGPage;
class GGConnection;
class GGContentElement;
class GGMappedLink;
class GGLink;
class GGCondition;
class GGAction;
class GGVariable;

class GGBasicProjectSerializer
{
public:
    GGBasicProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor);
    virtual ~GGBasicProjectSerializer();

    virtual bool saveProject(GGEditProject *project);

protected:
    virtual bool serializeProject(GGEditProject *project);
    virtual bool serializeScene(GGScene *scene);
    virtual bool serializePage(GGPage *page);
    virtual bool serializeConnection(GGConnection *connection);

    virtual bool serializeMedia(QVariant &v, const QString &path, GGEditProject *project);
    virtual bool serializeContent(QVariant &v, GGContentElement *elem);
    virtual bool serializeMappedLink(QVariant &v, const GGMappedLink &link, int idx);
    virtual bool serializeLink(QVariant &v, const GGLink &link);
    virtual bool serializeDecisionLink(QVariant &v, const GGLink &link, int idx);
    virtual bool serializeCondition(QVariant &v, const GGCondition &condition);
    virtual bool serializeAction(QVariant &v, const GGAction &action);
    virtual bool serializeVariable(QVariant &v, const GGVariable &var);

    virtual bool injectProjectData(GGEditProject *project, QVariantMap &v);
    virtual bool injectSceneData(GGScene *scene, QVariantMap &v);
    virtual bool injectPageData(GGPage *page, QVariantMap &v);
    virtual bool injectConnectionData(GGConnection *connection, QVariantMap &v);

    virtual bool finalizeScene(GGScene *scene);

protected:
    GGAbstractSerializationWriter *m_writer;
    GGSerializationProcessor *m_processor;
};

#endif // GGBASICPROJECTSERIALIZER_H
