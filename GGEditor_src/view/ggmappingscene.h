#ifndef GGMAPPINGSCENE_H
#define GGMAPPINGSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <model/ggmappedlink.h>
#include "ggselectionitem.h"
#include <model/ggconnectionslot.h>

class GGMappedContentPage;
class LinkRectItem;
class GGResizableRectItem;
class GGXorGraphicsRectItem;

class GGMappingScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GGMappingScene(QObject *parent = 0);
    ~GGMappingScene();

    GGConnectionSlot getSelectedSlot();

public slots:
    void setMappedElement(QPixmap p);
    void setConnections(GGMappedContentPage *page, QList<GGConnectionSlot> slts);
    void hoverItem(int idx);

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
    GGXorGraphicsRectItem *m_createItem;
    QGraphicsItem *m_pixItem;
    QPointF m_createStart;
    QList<GGResizableRectItem *> m_mapItems;
    int m_lastSelIdx;
    static const QColor s_defaultColor;
    static const QColor s_highColor;
    static const int s_penWidth = 3;
};

#endif // GGMAPPINGSCENE_H
