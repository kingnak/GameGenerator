#ifndef GGBASICPROJECTUNSERIALIZER_H
#define GGBASICPROJECTUNSERIALIZER_H

#include <io/ggabstractprojectunserializer.h>
#include <model/ggeditproject.h>

class GGAbstractUnserializationReader;
class GGUnserializationProcessor;
class GGEditProject;
class GGVariable;

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

protected:
    bool unserializeVariable(QVariant data, GGVariable &var);

protected:
    GGAbstractUnserializationReader *m_reader;
    GGUnserializationProcessor *m_processor;
    GGEditProject *m_project;
};

#endif // GGBASICPROJECTUNSERIALIZER_H
