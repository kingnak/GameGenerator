#ifndef GGVIEWMODEL_H
#define GGVIEWMODEL_H

#include <gg_definitions.h>
#include <QObject>
#include <QMap>

class GGEditModel;
class GGPage;
class GGConnection;
class GGViewPage;
class GGViewConnection;

class GGViewModel : public QObject
{
    Q_OBJECT
public:
    explicit GGViewModel(GGEditModel *model, QObject *parent = 0);
    ~GGViewModel();

    GGEditModel *editModel() { return m_model; }

    GGViewPage *getViewPageForPage(GGPage *page);
    GGViewConnection *getViewConectionForConnection(GGConnection *conn);

    bool registerPage(GGViewPage *page);

    void purgeRecycledPage(GGPage *page);
    void purgeRecycledConnection(GGConnection *conn);

    void notifyViewPageUpdate(GGViewPage *page);

signals:
    void viewPageRegistered(GGViewPage *vp);
    void viewPageUnregistered(GGViewPage *vp);
    void viewConnectionRegistered(GGViewConnection *vc);
    void viewConnectionUnregistered(GGViewConnection *vc);
    void pageUpdated(GGViewPage *vp);
    void viewPageUpdated(GGViewPage *vp);

private slots:
    void regPage(GGPage *page);
    void unregPage(GG::PageID, GGPage *page);
    void regConn(GGConnection *conn);
    void unregConn(GG::ConnectionID, GGConnection *conn);
    void updPage(GGPage *page);

private:
    GGEditModel *m_model;

    QMap<GGPage *, GGViewPage *> m_pageMap;
    QMap<GGConnection *, GGViewConnection *> m_connectionMap;

    QMap<GGPage *, GGViewPage *> m_pageRec;
    QMap<GGConnection *, GGViewConnection *> m_connectionRec;
};

#endif // GGVIEWMODEL_H
