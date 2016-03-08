#ifndef GGMAPPINGSCENE_H
#define GGMAPPINGSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <model/ggmappedlink.h>
#include "ggselectionitem.h"
#include <model/ggconnectionslot.h>

class GGMappedContentPage;
class LinkRectItem;

class GGMappingScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GGMappingScene(QObject *parent = 0);
    ~GGMappingScene();

public slots:
    void setMappedElement(QPixmap p);
    void setConnections(GGMappedContentPage *page, QList<GGConnectionSlot> slts);

signals:
    void addedItem(QRect pos);
    void movedItem(int idx, QRect pos);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private slots:
    void updateSelection();

private:
    void initSelectionItem();

private:
    friend class LinkRectItem;
    void itemMoved(LinkRectItem *itm);

private:
    GGSelectionItem *m_selItem;
    QGraphicsRectItem *m_createItem;
    QGraphicsItem *m_pixItem;
    QPointF m_createStart;
    QList<GGResizableItem *> m_mapItems;
    int m_lastSelIdx;
};

#endif // GGMAPPINGSCENE_H
