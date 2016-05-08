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
    virtual QString error() const;

    GGEditProject *loadedProject();
    GGEditProject *takeProject();

    bool unserializeProject(QVariant project);
    bool unserializeScene(QVariant scene);
    bool unserializePage(QVariant page);
    bool unserializeConnection(QVariant connection);
    bool unserializeForeignPage(QVariant page);
    bool unserializeForeignConnection(QVariant connection);
    bool finalizeUnserialization();

protected:
    virtual bool unserializeVariable(QVariant data, GGVariable &var);
    virtual bool unserializeMedia(QVariant data);
    virtual bool unserializeCondition(QVariant data, GGCondition &cond);
    virtual bool unserializeAction(QVariant data, GGAction &act);
    virtual bool unserializeContent(QVariant data, GGContentElement *&content);

    virtual bool unserializeMappedLink(QVariant data, GGMappedLink &link, GGPage *page, int &idx);
    virtual bool unserializeDecisionLink(QVariant data, GGLink &link, GGPage *page, int &idx);
    virtual bool unserializeLink(QVariant data, GGLink &link, GGPage *page, GGConnectionSlotData slot);

    virtual bool storeConnection(QVariantMap map, const QString &key, GGPage *page, GGConnectionSlotData slot);
    virtual bool storeConnection(QVariant data, GGPage *page, GGConnectionSlotData slot);

protected:
    GGAbstractUnserializationReader *m_reader;
    GGUnserializationProcessor *m_processor;
    GGEditProject *m_project;

    QMap<GG::ConnectionID, QPair<GGPage*, GGConnectionSlotData> > m_connSlots;
    QList<GGConnection *> m_conns;
};

#endif // GGBASICPROJECTUNSERIALIZER_H
