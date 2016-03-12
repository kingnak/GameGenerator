#ifndef GGRUNTIMEMODEL_H
#define GGRUNTIMEMODEL_H

#include "ggabstractmodel.h"
#include <QMap>

class GG_CORESHARED_EXPORT GGRuntimeModel : public GGAbstractModel
{
    Q_OBJECT
public:
    explicit GGRuntimeModel(GGAbstractFactory *factory, QObject *parent = 0);
    ~GGRuntimeModel();

    GGAbstractFactory *factory();

    GGPage *getPage(GG::PageID id);
    GGConnection *getConnection(GG::ConnectionID id);

    QList<GGPage *> getPages();
    QList<GGConnection *> getConnections();

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
    GGAbstractFactory *m_factory;
    QMap<GG::PageID, GGPage *> m_pages;
    QMap<GG::ConnectionID, GGConnection *> m_connections;
    QMap<QString, GGVariable> m_variables;
};

#endif // GGRUNTIMEMODEL_H
