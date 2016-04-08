#ifndef GGRUNTIMEMODEL_H
#define GGRUNTIMEMODEL_H

#include "ggabstractmodel.h"
#include <QMap>

class GG_CORESHARED_EXPORT GGRuntimeModel : public GGAbstractModel
{
    Q_OBJECT
public:
    explicit GGRuntimeModel(GGAbstractMediaResolver *resolver, QObject *parent = 0);
    ~GGRuntimeModel();

    GGAbstractMediaResolver *mediaResolver();

    GGScene *getScene(GG::SceneID id) const;
    GGPage *getPage(GG::PageID id) const;
    GGConnection *getConnection(GG::ConnectionID id) const;

    QList<GGScene *> getScenes() const;
    QList<GGPage *> getPages() const;
    QList<GGConnection *> getConnections();

    bool registerSceneWithId(GGScene *scene);
    bool registerPageWithId(GGPage *page);
    bool registerConnectionWithId(GGConnection *conn);

    QSet<GGVariable> variables() const;
    QList<QString> variableNames() const;
    GGVariable variableByName(QString name) const;
    bool addVariable(GGVariable v);
    bool removeVariable(GGVariable v);

signals:
    void variablesUpdated();

protected:
    GGAbstractMediaResolver *m_resolver;
    QMap<GG::SceneID, GGScene *> m_scenes;
    QMap<GG::PageID, GGPage *> m_pages;
    QMap<GG::ConnectionID, GGConnection *> m_connections;
    QMap<QString, GGVariable> m_variables;
};

#endif // GGRUNTIMEMODEL_H
