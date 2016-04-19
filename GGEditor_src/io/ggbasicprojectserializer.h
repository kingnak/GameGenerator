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

class GGBasicProjectSerializer
{
public:
    GGBasicProjectSerializer(GGAbstractSerializationWriter *writer, GGSerializationProcessor *processor = NULL);
    virtual ~GGBasicProjectSerializer();

    virtual bool saveProject(GGEditProject *project);

protected:
    virtual bool serializeProject(GGEditProject *project);
    virtual bool serializeScene(GGScene *scene);
    virtual bool serializePage(GGPage *page);
    virtual bool serializeConnection(GGConnection *connection);

    virtual QVariant serializeContent(GGContentElement *elem);
    virtual QVariant serializeMappedLink(const GGMappedLink &link);
    virtual QVariant serializeLink(const GGLink &link);
    virtual QVariant serializeCondition(const GGCondition &condition);
    virtual QVariant serializeAction(const GGAction &action);

private:
    GGAbstractSerializationWriter *m_writer;
    GGSerializationProcessor *m_processor;
};

#endif // GGBASICPROJECTSERIALIZER_H
