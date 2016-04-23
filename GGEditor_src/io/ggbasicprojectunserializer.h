#ifndef GGBASICPROJECTUNSERIALIZER_H
#define GGBASICPROJECTUNSERIALIZER_H

#include <io/ggabstractprojectunserializer.h>
#include <model/ggconnectionslotdata.h>
#include <model/ggeditproject.h>
#include <QMap>

class GGAbstractUnserializationReader;
class GGUnserializationProcessor;
class GGEditProject;
class GGVariable;
class GGCondition;
class GGAction;
class GGContentElement;
class GGLink;
class GGMappedLink;

class GGBasicProjectUnserializer : public GGAbstractProjectUnserializer
{
public:
    GGBasicProjectUnserializer(const QString &basePath, const QString &fileName, GGAbstractUnserializationReader *reader, GGUnserializationProcessor *processor);
    virtual ~GGBasicProjectUnserializer();

    virtual bool load(QIODevice *device);

    GGEditProject *loadedProject();
    GGEditProject *takeProject();

    bool unserializeProject(QVariant project);
    bool unserializeScene(QVariant scene);
    bool unserializePage(QVariant page);
    bool unserializeConnection(QVariant connection);
    bool finalizeUnserialization();

protected:
    bool unserializeVariable(QVariant data, GGVariable &var);
    bool unserializeCondition(QVariant data, GGCondition &cond);
    bool unserializeAction(QVariant data, GGAction &act);
    bool unserializeContent(QVariant data, GGContentElement *&content);

    bool unserializeMappedLink(QVariant data, GGMappedLink &link, GGPage *page, int &idx);
    bool unserializeDecisionLink(QVariant data, GGLink &link, GGPage *page, int &idx);
    bool unserializeLink(QVariant data, GGLink &link, GGPage *page, GGConnectionSlotData slot);

    bool storeConnection(QVariantMap map, const QString &key, GGPage *page, GGConnectionSlotData slot);
    bool storeConnection(QVariant data, GGPage *page, GGConnectionSlotData slot);

protected:
    GGAbstractUnserializationReader *m_reader;
    GGUnserializationProcessor *m_processor;
    GGEditProject *m_project;

    QMap<GG::ConnectionID, QPair<GGPage*, GGConnectionSlotData> > m_connSlots;
    QList<GGConnection *> m_conns;
};

#endif // GGBASICPROJECTUNSERIALIZER_H
