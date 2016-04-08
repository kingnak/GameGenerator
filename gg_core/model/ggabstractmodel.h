#ifndef GGABSTRACTMODEL_H
#define GGABSTRACTMODEL_H

#include <gg_core_global.h>
#include <gg_definitions.h>
#include <QObject>
#include "ggvariable.h"
#include "ggsearch.h"

class GGScene;
class GGPage;
class GGConnection;
class GGAbstractFactory;
class GGAbstractMediaResolver;

class GG_CORESHARED_EXPORT GGAbstractModel : public QObject
{
    Q_OBJECT

public:
    enum PageSection {
        PageScene       = 0x0001,
        PageData        = 0x0002,
        Connections     = 0x0004,
        Content         = 0x0008,
        MappedLinks     = 0x0010,
        DecisionLinks   = 0x0020,
        Condition       = 0x0040,
        AllSections = PageData | Connections | Content | MappedLinks | DecisionLinks | Condition
    };
    Q_DECLARE_FLAGS(PageSections, PageSection)

    explicit GGAbstractModel(QObject *parent = 0) : QObject(parent) {}

    virtual GGAbstractMediaResolver *mediaResolver() = 0;

    virtual GGScene *getScene(GG::SceneID id) const = 0;
    virtual GGPage *getPage(GG::PageID id) const = 0;
    virtual GGConnection *getConnection(GG::ConnectionID id) const = 0;

    virtual QList<GGScene *> getScenes() const = 0;
    virtual QList<GGPage *> getPages() const = 0;
    virtual QList<GGConnection *> getConnections() = 0;

    virtual bool registerSceneWithId(GGScene *scene) = 0;
    virtual bool registerPageWithId(GGPage *page) = 0;
    virtual bool registerConnectionWithId(GGConnection *conn) = 0;

    virtual void notifyPageUpdate(GG::PageID id, PageSections sections);

    virtual QSet<GGVariable> variables() const = 0;
    virtual QList<QString> variableNames() const = 0;
    virtual GGVariable variableByName(QString name) const = 0;
    virtual bool addVariable(GGVariable v) = 0;
    virtual bool removeVariable(GGVariable v) = 0;

    GGSearchResult search(GGSearchRequest req) const;

signals:
    void sceneRegistered(GGScene *scene);
    void sceneUnregistered(GG::SceneID id, GGScene *scene);
    void pageRegistered(GGPage *page);
    void pageUnregistered(GG::PageID id, GGPage *page);
    void connectionRegistered(GGConnection *conn);
    void connectionUnregistered(GG::ConnectionID id, GGConnection *conn);
    void pageUpdated(GGPage *page, GGAbstractModel::PageSections sections);

protected:
    void setSceneId(GGScene *scene, GG::SceneID id);
    void setPageId(GGPage *page, GG::PageID id);
    void setConnectionId(GGConnection *connection, GG::ConnectionID id);
    void unsetModel(GGScene *scene) const;
    void unsetModel(GGPage *page) const;
    void unsetModel(GGConnection *connection) const;
    void resolveConnectionPages(GGConnection *connection, GGPage *source, GGPage *destination) const;
    void resolvePageScene(GGPage *page, GGScene *scene) const;
};

#endif // GGABSTRACTMODEL_H

