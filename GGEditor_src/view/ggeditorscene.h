#ifndef GGEDITORSCENE_H
#define GGEDITORSCENE_H

#include <QGraphicsScene>
#include <QMap>

class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGPageItem;
class GGConnectionItem;

class GGEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GGEditorScene(GGViewModel *model, QObject *parent = 0);

signals:

private slots:
    void pageReg(GGViewPage *p);
    void pageUnreg(GGViewPage *p);
    void connReg(GGViewConnection *c);
    void connUnreg(GGViewConnection *c);
    void pageUpd(GGViewPage *p);
    void pageViewUpd(GGViewPage *p);

private:
    GGViewModel *m_model;

    QMap<GGViewPage *, GGPageItem *> m_pageMap;
    QMap<GGViewConnection *, GGConnectionItem *> m_connMap;
};

#endif // GGEDITORSCENE_H
