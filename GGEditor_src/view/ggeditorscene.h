#ifndef GGEDITORSCENE_H
#define GGEDITORSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QRect>

class GGViewModel;
class GGViewPage;
class GGViewConnection;
class GGPageItem;
class GGConnectionItem;

class GGSelectionItem;

class GGEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GGEditorScene(GGViewModel *model, QObject *parent = 0);
    void itemMoved(GGPageItem *item);

signals:
    void pageMoved(GGViewPage *page, QRect r);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void pageReg(GGViewPage *p);
    void pageUnreg(GGViewPage *p);
    void connReg(GGViewConnection *c);
    void connUnreg(GGViewConnection *c);
    void pageUpd(GGViewPage *p);
    void pageViewUpd(GGViewPage *p);

    void updateSelectionItem();

private:
    GGViewModel *m_model;
    GGSelectionItem *m_selItem;

    QMap<GGViewPage *, GGPageItem *> m_pageMap;
    QMap<GGViewConnection *, GGConnectionItem *> m_connMap;
};

#endif // GGEDITORSCENE_H
