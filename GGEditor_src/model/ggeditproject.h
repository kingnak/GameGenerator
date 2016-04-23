#ifndef GGEDITPROJECT_H
#define GGEDITPROJECT_H

#include <model/ggruntimeproject.h>
#include <QDir>
#include <io/ggiofactory.h>


class GGEditModel;
class GGSceneMediaManager;

class GGEditProject : public GGRuntimeProject
{
public:
    GGEditProject(const QString &basePath, const QString &fileName, GGIOFactory::SerializationType saveType);
    ~GGEditProject();

    GGEditModel *editModel();
    GGSceneMediaManager *mediaManager();
    QDir basePath() const;
    QString fileName() const;
    GGIOFactory::SerializationType saveType() const;
    void setSaveType(GGIOFactory::SerializationType type);

private:
    GGEditModel *m_editModel;
    GGSceneMediaManager *m_manager;
    QDir m_basePath;
    QString m_fileName;
    GGIOFactory::SerializationType m_saveType;
};

#endif // GGEDITPROJECT_H
